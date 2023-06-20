#include "../inc/server/Server.hpp"

Server::Server(unsigned int host, int port){
    setHostPort(host, port);
    setUpServer();
}
Server::Server() {}

Server::~Server() {}
/*
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
    return (*this);
} */

int Server::getFd(void) const
{
    return (socketfd);
}

int Server::getPort(void) const
{
    return (this->_port);
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

void             Server::setUpServer()
{
	struct sockaddr_in			 srvaddr;
	int opt = 1;
	this->socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socketfd == -1)
	{
		std::cerr << RED << "Could not create server." << RESET << std::endl;
		return ;
	}
	fcntl(this->socketfd, F_SETFL, O_NONBLOCK);
    if (setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(int)) < 0)
	{
		std::cerr << RED << "Could not re addr." << RESET << std::endl;
		close(this->socketfd);
		return ;
	}
	memset((char *)&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons((unsigned short)this->_port);
	if (bind(this->socketfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
	{
		//::close(this->socketfd);
		std::cerr << RED << "Could not bind port " << this->_port << "." << RESET << std::endl;
		return ;
	}
	if (listen(this->socketfd, 100) < 0)
	{
		//::close(this->socketfd);
		std::cerr << RED << "Could not listen." << RESET << std::endl;
		return ;
	}
	std::cout << GREEN << "Setting up " << this->_host << ":" << this->_port << "..." << RESET << std::endl;
}
