#pragma once

#include <iostream>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <arpa/inet.h>
#include "../utils/Utils.hpp"
#include "../error/ServerException.hpp"

class ServerException;
class Server 
{
    protected:
    	int					         socketfd;
        unsigned int                _host;
        int                         _port;
        ServerException             _serverException;
    public:
        Server(unsigned int host, int port);
        Server();
        Server(const Server &server);
        Server &operator=(const Server &server);
        ~Server();

        int             getFd() const;
        unsigned int	getHost() const;
		int				getPort() const;
        void            setHostPort(unsigned int host, int port);
        void            setUpServer();
        void            setUpServerMessage();
      
};
