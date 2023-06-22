#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <cstring>
#include <csignal>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "Server.hpp"
#include "Client.hpp"
#include "../entity/HttpScope.hpp"
#include "../error/ClusterException.hpp"

class Server;
class HttpScope;
class ClusterException;
class Cluster
{
    private:
        HttpScope*                  httpScope;
        std::map<int, Server *>     servers;
        int                         max_fd;
        fd_set                      readFds;
        fd_set                      supReadFds;
        fd_set                      supWriteFds;
        fd_set                      writeFds;
        int                         selected;
        int                         isMulti;
        int                         isFav;
        int                         loopControl;
        int                         status;
        size_t                      contentLen;
        std::map<int, Client *>     clients;
        std::string                 method;
        std::string                 favicon;
        std::string                 _response;
        std::string                 MultiBody;
        std::string                 body;
        ClusterException            _clusterException;
    public:
        Cluster();
        ~Cluster();

        int     setUpCluster(HttpScope *http);
        void    run();
        void    accept_section();
        void    recv_section();
        void    send_section();
        void	close_connection(std::map<int, Client *>::iterator it);
        void	findMaxFd();
        void    cleanServers();
        void    cleanClients();
        void    cleanAll();
};