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
#include "../entity/Request.hpp"
#include "../parser/ParserRequest.hpp"
#include "../entity/Response.hpp"
#include "../function/ServerFunc.hpp"


# define RECV_SIZE 65536

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

        long            getFd() const;
        t_listen        getListen() const;
        int             setUpSocket();
        void            setAddr();
        void            clean();
        void            close(int socket);
        long            accept();
        void            process(long socket, HttpScope *httpScope);
        void            processChunk(long socket);
        int             send(long socket);
        int             recv(long socket);
        ServerScope*    getServerForRequest(t_listen& address, HttpScope* http);
        LocationScope*  getLocationForRequest(ServerScope *matchedServerScope, const std::string& path); 
};
