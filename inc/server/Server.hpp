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

class Server 
{
    protected:
    	int					         socketfd;
        unsigned int                _host;
        int                         _port;

    public:
        Server(unsigned int host, int port);
        Server();
        //Server(const Server &server);
        //Server &operator=(const Server &server);
        ~Server();

        int             getFd() const;
        unsigned int	getHost() const;
		int				getPort() const;
        void            setUpServer();
        void            setHostPort(unsigned int host, int port);
      
};
