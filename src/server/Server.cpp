#include "../inc/server/Server.hpp"

Server::Server()
{

}

Server::~Server()
{
}

Server::Server(const t_listen &_listen)
{
    this->_listen = _listen;
    fd = -1;
    this->setAddr();
    this->setUpSocket();
}

void Server::setAddr(void)
{
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(_listen.host);
    addr.sin_port = htons(_listen.port);
}

int Server::setUpSocket()
{
    fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET-->ipv4   SOCK_STREAM-->TCP
    if (fd == -1)
    {
        std::cerr << "Could not create server." << std::endl; //hatalar ortak bir yerden yönetilecek
        return (-1);
    }
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) // ip:port (192.168.1.1:443) (host)ip ve port arasındaki bağlantıyı kurar
    {
        std::cerr << "Could not bind port " << _listen.port << "." << std::endl;
        return (-1);
    }
    if (listen(fd, 10) == -1) // aynı anda max 10 bağlantı kabul etmeye hazır
    {
        std::cerr << "Could not listen." << std::endl;
        return (-1);
    }
    return (0);
}

long Server::accept(void)
{
    long client_fd;

    client_fd = ::accept(fd, NULL, NULL); // client_fd üzerinden iletişim kurulabilir.
    if (client_fd == -1)
        std::cerr << "Could not create socket. " << std::endl;
    else
    {
        fcntl(client_fd, F_SETFL, O_NONBLOCK); // socket ayarlarını bloklanmamış olarak değiştiriyoruz.
        _requests.insert(std::make_pair(client_fd, ""));
    }
    return (client_fd);
}

void    Server::process(long socket, HttpScope& http)
{
    Response response;
    ServerScope *matchedServer;
    LocationScope *matchedLocation;

    //chunked request ayrı bir fonksiyonda işlem görecek.
    if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos && _requests[socket].find("Transfer-Encoding: chunked") < _requests[socket].find("\r\n\r\n"))
        this->processChunk(socket);

    if (_requests[socket].size() < 1000)
        std::cout << "\nRequest :" << std::endl
                  << "[" << _requests[socket] << "]" << std::endl;
    else
        std::cout << "\nRequest :" << std::endl
                  << "[" << _requests[socket].substr(0, 1000) << "..." << _requests[socket].substr(_requests[socket].size() - 10, 15) << "]" << std::endl;

    if (_requests[socket] != "")
    {
        // Request request(_requests[socket]); // aldığımız isteği parçalamak üzere Request class'a gönderiyoruz.
        Request *request;
        ParserRequest parserRequest(_requests[socket]);

        parserRequest.parse();
        request = parserRequest.getRequest();

        matchedServer = this->getServerForRequest(this->_listen, request->getIp(), http);
        matchedLocation = this->getLocationForRequest(matchedServer, request->getPath());

        response.createResponse(request, matchedServer, matchedLocation);

        // socket,request olan map yapısının requestini siliyoruz
        _requests.erase(socket);
        // requeste cevap oluşturup map içinde socket,response şeklinde tutuyoruz.
        _requests.insert(std::make_pair(socket, response.getResponse()));

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
        const int chunk_size = std::stoul(pos != std::string::npos ? chunk_str.substr(0, pos) : chunk_str, nullptr, 16);

        // Chunk boyutu kadar veriyi body'ye ekler.
        body.append(chunk_str.substr(chunk_str.find('\n') + 1, chunk_size));
    }

    // İstek başlığı ve yeni body'yi birleştirip isteği günceller.
    _requests[socket] = request.substr(0, header_end + 4) + body + "\r\n\r\n";
}

int Server::recv(long socket)
{
    int ret;
    char buffer[RECV_SIZE] = {0};

    ret = ::recv(socket, buffer, RECV_SIZE - 1, 0); // ret alınan veri boyutu

    if (ret == 0 || ret == -1)
    {
        this->close(socket);
        if (!ret) // ret 0 olması durumu
            std::cout << "\rConnection was closed by client.\n"
                      << std::endl;
        else
            std::cout << "\rRead error, closing connection.\n"
                      << std::endl; // ret -1 olması durumu
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
    static std::map<long, size_t> sent;

    if (sent.find(socket) == sent.end()) // gönderilecek mesaj kalmadıysa, socket içi boşsa
        sent[socket] = 0;

    std::string str = _requests[socket].substr(sent[socket], RECV_SIZE);
    int ret = ::send(socket, str.c_str(), str.size(), 0);

    if (ret == -1)
    {
        this->close(socket);
        sent[socket] = 0;
        return (-1);
    }
    else
    {
        sent[socket] += ret; // ret gönderilmiş veri bunu sent[socket] eklediğimizde aşağıda karşılaştırma yapabiliriz
        if (sent[socket] >= _requests[socket].size())
        {
            _requests.erase(socket); // socketten gönderilen mesajı sileriz
            sent[socket] = 0;
            return (0);
        }
        else
            return (1);
    }
}

long Server::get_fd(void) const
{
    return (fd);
}

t_listen   Server::get_listen() const
{
    return (_listen);
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


ServerScope*        Server::getServerForRequest(t_listen& address, const std::string& hostname, HttpScope& http)
{
    std::vector<ServerScope *>  matchingServers;
    std::vector<ServerScope *>  serverScope;

    serverScope = http.getServers();
    for (std::vector<ServerScope *>::const_iterator it = serverScope.begin() ; it != serverScope.end(); it++)
    {
        if (address.host == (*it)->getListen().host && address.port == (*it)->getListen().port)
        {
            matchingServers.push_back(*it);
            for(size_t i = 0; i < (*it)->getServerName().size(); i++)
            {
                if((*it)->getServerName().at(i) == hostname)
                    return *it;
            }
        }
    }
    if (matchingServers.empty())
    {
        std::cerr << "there is no possible server" << std::endl;
        return NULL;
    }
    // If no server name matches, return the first matching server
    return matchingServers.front();
}


//benim yazdığım daha basic olan
LocationScope*  Server::getLocationForRequest(ServerScope *matchedServer, const std::string& path) 
{
    std::vector<LocationScope *> locationScope;

    locationScope = matchedServer->getLocations();
    for(std::vector<LocationScope *>::iterator it = locationScope.begin(); it != locationScope.end(); it++)
    {
        if(path == (*it)->getPath())
            return (*it);
    }
    return (*(matchedServer->getLocations().begin()));
}
// //chatgpt
// //yukarıdakinden daha gelişmiş bir formatta yazdı 
// LocationScope* Server::getLocationForRequest(ServerScope* matchedServer, const std::string& path)
// {
//     LocationScope* bestMatch = 0;  // En iyi eşleşme için LocationScope

//     // Tüm location'lar üzerinde gezin ve en iyi eşleşmeyi bul
//     for (std::vector<LocationScope*>::iterator it = matchedServer->getLocations().begin(); it != matchedServer->getLocations().end(); ++it) 
//     {
//         LocationScope* location = *it;

//         // Path'in location path'iyle eşleştiğini kontrol et
//         if (location->getPath() == path) {
//             bestMatch = location;
//             break;
//         }

//         // Eğer location regex kullanıyorsa, regex'e göre eşleşmeyi kontrol et
//         //çok abartı bu silebiliriz.
//         //Örneğin, "/user/[0-9]+" ifadesi, "/user/123", "/user/456", "/user/789", gibi "/user/" ile başlayan herhangi bir istek yolunu eşleştirebilir.
//         if (location->hasRegex()) {
//             if (std::regex_match(path, location->getRegex())) {
//                 if (!bestMatch || bestMatch->getPath().length() < location->getPath().length()) {
//                     bestMatch = location;
//                 }
//             }
//         }

//         // Eğer en uzun eşleşme prensibi kullanılıyorsa, path'in location path'iyle başladığından emin ol
//         //Örneğin, bir sunucuda /test ve /test/test2 isimli iki konum olsun. 
//         //Eğer gelen istek URL'si /test/test2/index.html ise, sunucu bu isteği işlerken /test/test2 konumunu seçer 
//         //çünkü bu, URL'nin en uzun eşleşmesidir.
//         if (location->usesLongestMatch() && path.find(location->getPath()) == 0) {
//             if (!bestMatch || bestMatch->getPath().length() < location->getPath().length()) {
//                 bestMatch = location;
//             }
//         }
//     }

//     // Eğer hiçbir eşleşme bulunamazsa, default location kullanılır
//     if (!bestMatch) {
//         bestMatch = matchedServer->getDefaultLocation();
//     }

//     return bestMatch;
// }



/**********************************************************************************/

void Server::noName(ServerScope *server, LocationScope *location, Request *request)
{
	std::vector<std::string> conf_index = server->getIndex();
	for (std::vector<std::string>::const_iterator it = conf_index.begin(); it != conf_index.end(); it++) 
    {
		std::vector<std::string>::const_iterator it2 = _index.begin();
		for (it2 = _index.begin(); it2 != _index.end(); it2++) 
        {
				if (*it == *it2)
					break;
		}
		if (it2 == _index.end())
			_index.push_back(*it);
	}

	this->_contentLocation = removeAdjacentSlashes(path);
	this->_path = removeAdjacentSlashes(server->getRoot() + location.getPath());
	std::string indexPath;
	if (!pathIsFile(this->_path) && method == "GET") 
    {
		if ((indexPath = this->addIndex(request)) != "") {
			config = config.getLocationForRequest(indexPath, locationName);
			this->_cgi_pass = config.getCgiPass();
			this->_cgi_param = config.getCgiParam();
		}
	}

}