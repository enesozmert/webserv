#include "../inc/server/Server.hpp"
Server::Server() {}

Server::Server(HttpScope* http, unsigned int host, int port){
    this->_http = http;
    setHostPort(host, port);
    setUpServer();
}

Server::~Server() {}

Server::Server(const Server &server)
{
	*this = server;
}

Server& Server::operator=(const Server &server)
{
	if (this == &server)
        return (*this);
    this->fd = server.fd;
    this->addr = server.addr;
    this->_port = server._port;
    this->_host = server._host;
    this->_http = server._http;
    return (*this);
}

int Server::getFd(void) const
{
    return (fd);
}

int Server::getPort(void) const
{
    return (this->_port);
}

HttpScope*     Server::getHttp() const
{
    return this->_http;
}

unsigned int Server::getHost(void) const
{
    return (this->_host);
}

void    Server::setHostPort(unsigned int host, int port)
{
    this->_host = host;
    this->_port = port;
}

void Server::setAddr()
{
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->_port);
    addr.sin_addr.s_addr = htonl(this->_host);//127.0.0.1--->2130706433
}

void             Server::setUpServer()
{
	int opt = 1;
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->fd == -1)
	{
		std::cerr << RED << "Could not create server." << RESET << std::endl;
		return ;
	}
	fcntl(this->fd, F_SETFL, O_NONBLOCK);
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(int)) == -1)
	{
		std::cerr << RED << "Could not re addr." << RESET << std::endl;
		return ;
	}
    setAddr();
	if (bind(this->fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		::close(this->fd);
		std::cerr << RED << "Could not bind port " << this->_port << "." << RESET << std::endl;
		return ;
	}
	if (listen(this->fd, 100) == -1)
	{
		::close(this->fd);
		std::cerr << RED << "Could not listen." << RESET << std::endl;
		return ;
	}
	std::cout << GREEN << "Setting up " << this->_host << ":" << this->_port << "..." << RESET << std::endl;
}
