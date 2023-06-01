#include "../inc/server/Server.hpp"

Server::Server() {
    fd = -1;
}

Server::~Server() {}

Server::Server(const t_listen &listen)
{
    this->_listen = listen;
    fd = -1;
}

long Server::getFd(void) const
{
    return (fd);
}

t_listen   Server::getListen() const
{
    return (_listen);
}

void Server::setAddr(void)
{
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(_listen.host);//127.0.0.1--->2130706433
    addr.sin_port = htons(_listen.port);
}

int Server::setUpSocket()
{
    fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET-->ipv4   SOCK_STREAM-->TCP
    if (fd == -1)
    {
        std::cerr << RED << "Could not create server." << RESET << std::endl; //hatalar ortak bir yerden yönetilecek
        return -1;
    }
    this->setAddr();
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) // ip:port (192.168.1.1:443) (host)ip ve port arasındaki bağlantıyı kurar
    {
        std::cerr << RED << "Could not bind port " << _listen.port << "." << RESET << std::endl;
        return -1;
    }
    if (listen(fd, 10) == -1) // aynı anda max 10 bağlantı kabul etmeye hazır
    {
        std::cerr << RED << "Could not listen." << RESET << std::endl;
        return -1;
    }
    return (0);
}

long Server::accept(void)
{
    std::cout << YELLOW << "\nAccepting..." << RESET << std::endl;
    long client_fd;

    client_fd = ::accept(fd, NULL, NULL); // client_fd üzerinden iletişim kurulabilir.
    if (client_fd == -1)
        std::cerr << RED << "Could not create socket. " << RESET << std::endl;
    fcntl(client_fd, F_SETFL, O_NONBLOCK); // socket ayarlarını bloklanmamış olarak değiştiriyoruz.
    _requests.insert(std::make_pair(client_fd, ""));
    return (client_fd);
}

void    Server::process(long socket, HttpScope* http)
{
    Response        response;
    ServerScope     *matchedServer;
    LocationScope   *matchedLocation;

    //chunked request ayrı bir fonksiyonda işlem görecek.
    if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos && _requests[socket].find("Transfer-Encoding: chunked") < _requests[socket].find("\r\n\r\n"))
        this->processChunk(socket);

    if (_requests[socket].size() < 1000)//request ekrana basıyoruz
        std::cout << RED << "\nRequest :" << std::endl << "[" << _requests[socket] << "]" << RESET << std::endl;
    else
        std::cout << RED << "\nRequest :" << std::endl << "[" << _requests[socket].substr(0, 1000) << "..." << _requests[socket].substr(_requests[socket].size() - 10, 15) << "]" << RESET << std::endl;

    if (_requests[socket] != "")
    {
        Request *request;
        ParserRequest parserRequest(_requests[socket]);// aldığımız isteği parçalamak üzere Request class'a gönderiyoruz.

        parserRequest.parse();
        request = parserRequest.getRequest();

        matchedServer = this->getServerForRequest(this->_listen, http);
        this->getLocationForRequest(matchedServer, request->getPath());
        if (matchedServer == NULL || this->locationScopeIndex == -1)
        {
            _requests.erase(socket);
            _requests.insert(std::make_pair(socket, ""));
            return ;
        }
        else
        {
            matchedLocation = matchedServer->getLocations().at(this->locationScopeIndex);
            response.createResponse(request, matchedServer, matchedLocation);

            // socket,request olan map yapısının requestini siliyoruz
            _requests.erase(socket);
            // requeste cevap oluşturup map içinde socket,response şeklinde tutuyoruz.
            _requests.insert(std::make_pair(socket, response.getResponse()));
        }
        //not:
        //www.google.com:80
        //192.282.23.2:
        //192.80.808.1:8080
    }
}

//Chunked request, HTTP protokolünde kullanılan bir veri transfer yöntemidir.
//Bu yöntemde, gönderilecek veri belirli boyutlarda parçalara ayrılır ve her bir parça ayrı bir "chunk" olarak gönderilir. 
//Bu sayede, verinin tamamı gönderilmeden önce tüm parçaların bir arada toplanması beklenmez,
//böylece büyük boyutlu verilerin transferi daha verimli bir şekilde gerçekleştirilebilir.
//Veri gönderimi tamamlandığında, son "chunk" gönderildikten sonra bir "terminator" chunk eklenir 
//ve böylece alıcı taraf, verinin tamamının gönderildiğini anlar.
//body kısmı çok uzunsa bu şekilde parçalı olarak alıp sonra birleştiririz.
/* void Server::processChunk(long socket)
{
    std::string head = _requests[socket].substr(0, _requests[socket].find("\r\n\r\n"));
    std::string chunks = _requests[socket].substr(_requests[socket].find("\r\n\r\n") + 4, _requests[socket].size() - 1);
    std::string subchunk = chunks.substr(0, 100);
    std::string body = "";
    int chunksize = strtol(subchunk.c_str(), NULL, 16);
    size_t i = 0;

    while (chunksize)
    {
        i = chunks.find("\r\n", i) + 2;
        body += chunks.substr(i, chunksize);
        i += chunksize + 2;
        subchunk = chunks.substr(i, 100);
        chunksize = strtol(subchunk.c_str(), NULL, 16);
    }

    _requests[socket] = head + "\r\n\r\n" + body + "\r\n\r\n";
} */

//chatgpt
void Server::processChunk(long socket)
{
    const std::string& request = _requests[socket];

    // İstek başlığı (header) ile body arasındaki bölgeyi bulur.
    const std::string::size_type header_end = request.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        // İstek başlığı tamamlanmadığı için işlem yapılamaz.
        return;
    }

    // Body'yi "chunked" encoding kullanarak parçalara ayırır.
    std::istringstream body_stream(request.substr(header_end + 4));
    std::string chunk_str;
    std::string body;
    while (std::getline(body_stream, chunk_str), !chunk_str.empty()) {
        // Chunk boyutunu hex değerinden decimal değere çevirir.
        const std::string::size_type pos = chunk_str.find(';');
        const int chunk_size = stoul_cxx98(pos != std::string::npos ? chunk_str.substr(0, pos) : chunk_str, NULL, 16);

        // Chunk boyutu kadar veriyi body'ye ekler.
        body.append(chunk_str.substr(chunk_str.find('\n') + 1, chunk_size));
    }

    // İstek başlığı ve yeni body'yi birleştirip isteği günceller.
    _requests[socket] = request.substr(0, header_end + 4) + body + "\r\n\r\n";
}

int Server::recv(long socket)
{
    std::cout << YELLOW <<  "\nReceiving..." << RESET << std::endl;
    int Recieved;
    char buffer[RECV_SIZE] = {0};

    Recieved = ::recv(socket, buffer, RECV_SIZE - 1, 0); // Recieved alınan veri boyutu

    if (Recieved == 0 || Recieved == -1)
    {
        this->close(socket);
        if (!Recieved)
            std::cout << RED << "\rConnection was closed by client.\n" << RESET << std::endl;
        else
            std::cout << RED << "\rRead error, closing connection.\n" << RESET << std::endl;
        return (-1);
    }

    _requests[socket] += std::string(buffer); // bufferda tutulan alınmış veriyi _requests[socket] değeri olarak ekliyoruz.

    size_t i = _requests[socket].find("\r\n\r\n"); //"\\r\\n\\r\\n" (HTTP istek başlığı ve gövdesi arasındaki ayırıcı) bu konumun indeksi i
    if (i != std::string::npos)                   // std::string::npos olmayan bir i değişkeni, _requests[socket] öğesinde "\\r\\n\\r\\n" ifadesinin bulunduğu konumu temsil eder
    {
        if (_requests[socket].find("Content-Length: ") == std::string::npos) //"Content-Length: " bulunduysa
        {
            if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos) //"Transfer-Encoding: chunked" bulunmadıysa
            {
                if (_requests[socket].compare(_requests[socket].length() - 5, 5, "0\r\n\r\n") == 0)
                    return (0);
                else
                    return (1);
            }
            else
                return (0);
        }
        // content-length başladığı yere indeks döndürürsek oradan karşılık gelen değeri substr ile çekiyoruz.
        size_t len = std::atoi(_requests[socket].substr(_requests[socket].find("Content-Length: ") + 16, 10).c_str());
        // i-->istek başlığı uzunluğu
        // len-->istek gövdesi uzunluğu
        //"\\r\\n\\r\\n"-->4 seperator bunları toplayıp isteğin hepsi elimize ulaştı mı kontrol ederiz
        if (_requests[socket].size() >= len + i + 4)
            return (0);
        else
            return (1);
    }

    return (1);
}

int Server::send(long socket)
{
    std::cout << YELLOW << "\nSending..." << RESET << std::endl;
    static std::map<long, size_t> sent;

    if (sent.find(socket) == sent.end()) // gönderilecek mesaj kalmadıysa, socket içi boşsa
        sent[socket] = 0;
    
    if (1 && sent[socket] == 0)
	{
		if (_requests[socket].size() < 1000)
			std::cout << "\rResponse :                " << std::endl << "[" << GREEN << _requests[socket] << RESET << "]\n" << std::endl;
		else
			std::cout << "\rResponse :                " << std::endl << "[" << GREEN << _requests[socket].substr(0, 1000) << "..." << _requests[socket].substr(_requests[socket].size() - 10, 15) << RESET << "]\n" << std::endl;
	}

    std::string str = _requests[socket].substr(sent[socket], RECV_SIZE);
    int SentData = ::send(socket, str.c_str(), str.size(), 0);//gönderilen veri boyutu döner

    if (SentData == -1)
    {
        this->close(socket);
        sent[socket] = 0;
        return (-1);
    }
    sent[socket] += SentData; // ret gönderilmiş veri bunu sent[socket] eklediğimizde aşağıda karşılaştırma yapabiliriz
    if (sent[socket] >= _requests[socket].size())
    {
        _requests.erase(socket); // socketten gönderilen mesajı sileriz
        sent[socket] = 0;
        return (0);
    }
    return (1);
}

void Server::close(int socket)
{
    if (socket > 0)
        ::close(socket);
    _requests.erase(socket);
}

void Server::clean()
{
    if (fd > 0)
        ::close(fd);
    fd = -1;
}



/****************************************************************/


ServerScope*        Server::getServerForRequest(t_listen& address, HttpScope* http)
{
    std::vector<ServerScope *>  serverScope;

    serverScope = http->getServers();
    for (std::vector<ServerScope *>::const_iterator it = serverScope.begin() ; it != serverScope.end(); it++)
    {
        if (address.host == (*it)->getListen().host && address.port == (*it)->getListen().port)
            return(*it);
    }
    std::cerr << RED << "There is no possible server" << RESET << std::endl;
    return NULL;
}


//benim yazdığım daha basic olan
void  Server::getLocationForRequest(ServerScope *matchedServerScope, const std::string& path) 
{
    this->locationScopeIndex = 0;

    this->locationScopeIndex = getMatchLocationPathIndex(matchedServerScope, path);
    std::cout << "locationScopeIndex : " << locationScopeIndex << std::endl;
    if(this->locationScopeIndex == -1)
        std::cerr << RED << "There is no possible location" << RESET << std::endl;
    //locationScopeIndex = getDefaultLocationPathIndex(matchedServerScope);
    //locationScopeIndex = getLongestLocationPathIndex(matchedServerScope);
}