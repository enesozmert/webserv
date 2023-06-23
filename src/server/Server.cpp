#include "../inc/server/Server.hpp"

Server::Server(unsigned int host, int port){
    setHostPort(host, port);
    setUpServer();
}
Server::Server() {}

Server::~Server() {}

Server::Server(const Server &server)
{
	*this = server;
}

Server& Server::operator=(const Server &server)
{
	if (this == &server)
        return (*this);
    this->socketfd = server.socketfd;
    this->_serverException = server._serverException;
    this->_port = server._port;
    this->_host = server._host;
    return (*this);
}

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
		this->_serverException.run(200);
		return ;
	}
	fcntl(this->socketfd, F_SETFL, O_NONBLOCK);
    if (setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(int)) < 0)
	{
		this->_serverException.run(201);
		close(this->socketfd);
		return ;
	}
	memset((char *)&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(this->_host);
	srvaddr.sin_port = htons((unsigned short)this->_port);
	if (bind(this->socketfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
	{
		::close(this->socketfd);
		this->_serverException.run(202);
		return ;
	}
	if (listen(this->socketfd, 100) < 0)
	{
		::close(this->socketfd);
		this->_serverException.run(203);
		return ;
	}
	setUpServerMessage();
}

void	Server::setUpServerMessage()
{
	std::cout << GREEN << "Setting up " << this->_host << ":" << this->_port << "..." << RESET << std::endl;
}