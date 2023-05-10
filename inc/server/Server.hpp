#pragma once

#include <iostream>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <map>

#include "../entity/Listen.hpp"
#include "../entity/HttpScope.hpp"

# define RECV_SIZE 65536

class HttpScope;
class Server {
    private:
        std::map<long, std::string>	_requests;
    	t_listen					_listen;//host and port
    	long						fd;
    	struct sockaddr_in			addr;//host ve port ayarlanacak

    public:
        Server();
        Server(const t_listen &listen);
        ~Server();

        long	get_fd();

        int     setUpServer();
        void    setAddr();
        void    clean();
        void    close(int socket);

        long    accept();
        void	process(long socket, HttpScope &httpScope);
        void	processChunk(long socket);
        int		send(long socket);
        int		recv(long socket);
};
