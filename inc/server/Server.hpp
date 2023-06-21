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

<<<<<<< HEAD

# define RECV_SIZE 4096

class Server {
    private:
        std::map<long, std::string>	_requests;
    	long						_fd;
    	t_listen					_listen;
    	struct sockaddr_in			_addr;
        int                         _locationScopeIndex;
=======
class Server 
{
    protected:
    	int					         socketfd;
        unsigned int                _host;
        int                         _port;
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61

    public:
        Server(unsigned int host, int port);
        Server();
<<<<<<< HEAD
        Server(const t_listen &listen);
        Server(const Server &server);
=======
        //Server(const Server &server);
        //Server &operator=(const Server &server);
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
        ~Server();

        int             getFd() const;
        unsigned int	getHost() const;
		int				getPort() const;
        void            setUpServer();
        void            setHostPort(unsigned int host, int port);
      
};
