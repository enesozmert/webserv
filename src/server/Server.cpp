#include "../inc/server/Server.hpp"

Server::Server():_fd(-1) {}

Server::~Server() {}

Server::Server(const t_listen &listen) :_fd(-1), _listen(listen)
{
    _addr.sin_family = 0;
    _addr.sin_port = 0;
    _addr.sin_addr.s_addr = 0;
}

Server::Server(const Server &server)
{
	*this = server;
}

long Server::getFd(void) const
{
    return (_fd);
}

t_listen   Server::getListen() const
{
    return (_listen);
}

void Server::setAddr(void)
{
    memset((char *)&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_listen.port);
    _addr.sin_addr.s_addr = htonl(_listen.host);
}

int Server::setUpSocket()
{
    int opt = 1;
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cerr << RED << "Could not re addr." << RESET << std::endl; //hatalar ortak bir yerden yönetilecek
        return (-1);
    }
    if (_fd == -1)
    {
        std::cerr << RED << "Could not create server." << RESET << std::endl; //hatalar ortak bir yerden yönetilecek
        return -1;
    }
    this->setAddr();
    if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0) // ip:port (192.168.1.1:443) (host)ip ve port arasındaki bağlantıyı kurar
    {
        ::close(_fd);
        std::cerr << RED << "Could not bind port " << _listen.port << "." << RESET << std::endl;
        return -1;
    }
    if (listen(_fd, 4096) == -1)
    {
        ::close(_fd);
        std::cerr << RED << "Could not listen." << RESET << std::endl;
        return -1;
    }
    return (0);
}

long Server::accept()
{
    std::cout << YELLOW << "\nAccepting..." << RESET << std::endl;
    long client_fd;

    client_fd = ::accept(_fd, NULL, NULL); // client_fd üzerinden iletişim kurulabilir.
    if (client_fd == -1)
    {
        std::cerr << RED << "Could not create socket. " << RESET << std::endl;
        return(-1);
    }
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

    if (_requests[socket].size() < 1000)
        std::cout << RED << "\nRequest :" << std::endl << "[" << _requests[socket] << "]" << RESET << std::endl;
    else
        std::cout << RED << "\nRequest :" << std::endl << "[" << _requests[socket].substr(0, 1000) << "..." << "]" << RESET << std::endl;

    if (_requests[socket] != "")
    {
        Request *request;
        ParserRequest parserRequest(_requests[socket]);// aldığımız isteği parçalamak üzere Request class'a gönderiyoruz.

        parserRequest.parse();
        request = parserRequest.getRequest();

        matchedServer = this->getServerForRequest(this->_listen, http);
        this->getLocationForRequest(matchedServer, request->getPath());
        if (matchedServer == NULL || this->_locationScopeIndex == -1)
        {
            _requests.erase(socket);
            _requests.insert(std::make_pair(socket, ""));
            return ;
        }
        else
        {
            matchedLocation = matchedServer->getLocations().at(this->_locationScopeIndex);
            response.createResponse(request, matchedServer, matchedLocation);
            // socket,request olan map yapısının requestini siliyoruz
            _requests.erase(socket);
            // requeste cevap oluşturup map içinde socket,response şeklinde tutuyoruz.
            _requests.insert(std::make_pair(socket, response.getResponse()));
        }
    }
}

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
    int recieved_data_size = 0;
    bool received = false;
    char buffer[RECV_SIZE] = {0};

    while(!received || recieved_data_size > 0)
    {
        received = true;
        recieved_data_size = ::recv(socket, buffer, RECV_SIZE - 1, 0);
        _requests[socket] += std::string(buffer, recieved_data_size);
        memset(buffer, 0, sizeof(buffer));
        if (recieved_data_size < RECV_SIZE - 1)
        {
            _requests[socket] += std::string(buffer, recieved_data_size);
            memset(buffer, 0, sizeof(buffer));
            received = false;
            break;
        }
    } 
    if (recieved_data_size == 0 || recieved_data_size == -1)
    {
        this->close(socket);
        if (!recieved_data_size)
            std::cout << RED << "\rConnection was closed by client.\n" << RESET << std::endl;
        else
            std::cout << RED << "\rRead error, closing connection.\n" << RESET << std::endl;
        return (-1);
    }
    return (0);
}

int Server::send(long socket)
{
    std::cout << YELLOW << "\nSending..." << RESET << std::endl;
    int send_data_size = 0;
    bool sended = false;
    std::string response = _requests[socket];
    static std::map<long, size_t> sent;
    sent[socket] = 0;
    while (!sended || send_data_size > 0)
    {
        sended = true;
        std::string str = response.substr(sent[socket], RECV_SIZE);
        send_data_size = ::send(socket, str.c_str(), str.size(), 0);
        if (send_data_size == -1)
            return (-1);
        sent[socket] += send_data_size;
        str = "";
        if (send_data_size == 0 || send_data_size == -1)
            break ;
    }
    if (sent[socket] >= _requests[socket].size())
    {
        if (_requests[socket].size() < 500)
			std::cout << GREEN << "\rResponse :                " << std::endl << "[" << _requests[socket] << RESET << "]\n" << RESET << std::endl;
		else
			std::cout << GREEN << "\rResponse :                " << std::endl << "[" << _requests[socket].substr(0, 500) << "..." << "]\n" << RESET << std::endl;
    }
    sent[socket] = 0;
    this->close(socket);
    _requests.erase(socket); 
    if (send_data_size == -1)
        return (-1);
    return 0;
}

void Server::close(int socket)
{
    if (socket > 0)
        ::close(socket);
    _requests.erase(socket);
}

void Server::clean()
{
    if (_fd > 0)
        ::close(_fd);
    _fd = -1;
}

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

void  Server::getLocationForRequest(ServerScope *matchedServerScope, const std::string& path) 
{
    this->_locationScopeIndex = 0;

    this->_locationScopeIndex = getMatchLocationPathIndex(matchedServerScope, path);
    if (this->_locationScopeIndex == -1)
        std::cerr << RED << "There is no possible location" << RESET << std::endl;
}