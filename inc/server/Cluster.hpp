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
        size_t                      ContentLen;
        std::map<int, Client *>     clients;
        std::string                 method;
        std::string                 favicon;
        std::string                 _response;
        std::string                 MultiBody;
        std::string                 body;

<<<<<<< HEAD
private:
    HttpScope *httpScope; // http class gelecek
    std::map<long, Server> servers;
    std::map<long, Server *> sockets; // her server üzerinden kurulan socketleri tutacak
    std::vector<int> ready;           // veri gönderimine hazır socketler(writing_set'e eklenmiş)
    fd_set fdMaster;                 //(tüm fd'lerin tutulduğu set)
    long maxFd;                      // select() için ve kontrol amaçlı gerekli
    fd_set readingSet;               // içinden veri okunmaya hazır fd seti
    fd_set writingSet;               // içine veri yazılmaya hazır fd seti
    struct timeval timeout;           // select() için zaman aşımı süresi
    int selectReturnValue;          // select() return değeri

public:
    Cluster();
    ~Cluster();
    Cluster(const Cluster &cluster);

    int setUpCluster(HttpScope *http);
    void run();
    void cleanServers();
    void cleanSockets();
    void cleanReady();
    void select_section(); // havuzdan fd seçimi yapılır
    void accept_section(); // client'a hizmet için hazır bir socket oluşturulur
    void recv_section();
    void send_section();
    void cleanAll();
=======
    public:
        Cluster();
        ~Cluster();
        /*
        Cluster(const Cluster &cluster);
        Cluster &operator=(const Cluster &cluster); */
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61

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