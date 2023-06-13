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

};