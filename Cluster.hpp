#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "Server.hpp"
#include "Config.hpp"
#include "webserv.hpp"

class Cluster
{
private:
    Config						config;//http class gelecek
    std::map<long, Server>      servers;
    std::map<long, Server *>    sockets;//her server üzerinden kurulan socketleri tutacak
    std::vector<int>            ready;//veri gönderimine hazır socketler(writing_set'e eklenmiş)
    fd_set                      fd_master;//(tüm fd'lerin tutulduğu set)
    long						max_fd;//select() için ve kontrol amaçlı gerekli
    fd_set		                reading_set;//içinden veri okunmaya hazır fd seti
	fd_set		                writing_set;//içine veri yazılmaya hazır fd seti
	struct timeval	            timeout;//select() için zaman aşımı süresi
    int                         select_return_value;//select() return değeri

public:
    Cluster();
    ~Cluster();

    //setUp-->run-->select-->accept-->recv-->send
    int		setUpCluster();
	void	run();
    void	select_section();//havuzdan fd seçimi yapılır
    void 	accept_section();//client'a hizmet için hazır bir socket oluşturulur
    void	recv_section();
    void	send_section();
};


#endif