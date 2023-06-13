#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <cstring>
#include <csignal>

#include "Server.hpp"
#include "../entity/HttpScope.hpp"

class Server;
class HttpScope;
class Cluster
{

private:
    HttpScope*                  httpScope; //http class gelecek
    std::map<long, Server>      servers;
    std::map<long, Server *>    sockets; //her server üzerinden kurulan socketleri tutacak
    std::vector<int>            ready; //veri gönderimine hazır socketler(writing_set'e eklenmiş)
    long                        max_fd;// select() için ve kontrol amaçlı gerekli
    fd_set                      fd_master;//(tüm fd'lerin tutulduğu set)
    fd_set                      reading_set;
	fd_set                      writing_set;
	struct timeval              timeout;
    int                         select_return_value;

public:
    Cluster();
    ~Cluster();
    Cluster(const Cluster &cluster);
    Cluster &operator=(const Cluster &cluster);

    int     setUpCluster(HttpScope *http);
    void    run();
    void    cleanServers();
    void    cleanSockets();
    void    cleanReady();
    void    select_section(); // havuzdan fd seçimi yapılır
    void    accept_section(); // client'a hizmet için hazır bir socket oluşturulur
    void    recv_section();
    void    send_section();
    void    cleanAll();

};