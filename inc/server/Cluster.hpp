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

class Server;
class HttpScope;
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
        size_t                      postLen;
        std::map<int, Client *>     clients;

    public:
        Cluster();
        ~Cluster();
        Cluster(const Cluster &cluster);
        Cluster &operator=(const Cluster &cluster);

        int     setUpCluster(HttpScope *http);
        void    run();
        void    cleanServers();
        void    cleanClients();
        void    accept_section();
        void    recv_section();
        void    send_section();
        void    cleanAll();
        void	close_connection(std::map<int, Client *>::iterator it);
        void	findMaxFd();
};