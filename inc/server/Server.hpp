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
#include "../inc/entity/HttpScope.hpp"

class Server 
{
    private:
    	int					         fd;
    	struct sockaddr_in			 addr;
        unsigned int                _host;
        int                         _port;
        HttpScope*                  _http;

    public:
        Server();
        Server(HttpScope* http, unsigned int host, int port);
        Server(const Server &server);
        Server &operator=(const Server &server);
        ~Server();

        int             getFd() const;
        unsigned int	getHost() const;
		int				getPort() const;
        HttpScope*      getHttp() const;
        void            setUpServer();
        void            setAddr();
        void            setHostPort(unsigned int host, int port);
      
};
