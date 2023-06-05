#include "../inc/server/Cluster.hpp"

Cluster::Cluster() {
	FD_ZERO(&fd_master);
	FD_ZERO(&writing_set);
	FD_ZERO(&reading_set);
}

Cluster::~Cluster() {
	std::cout << YELLOW << "Cluster destruct" << RESET << std::endl;
	this->cleanServers();
	this->cleanSockets();
	this->cleanReady();
	FD_ZERO(&fd_master);
	FD_ZERO(&writing_set);
	FD_ZERO(&reading_set);
}

Cluster::Cluster(const Cluster &cluster)
{
	*this = cluster;
}

Cluster& Cluster::operator=(const Cluster &cluster)
{
	if (this == &cluster)
        return (*this);
	this->servers = cluster.servers;
	this->sockets= cluster.sockets;
    return (*this);
}

int Cluster::setUpCluster(HttpScope* http)
{
	this->httpScope = http;
    //serverların hepsini vector olarak tutuyorduk. Onları çektik.
	std::vector<t_listen>	listens;
	listens = this->httpScope->getListens();

	//olası hataları önlemek için bütün fd'ler 0'a eşitlenir.
	FD_ZERO(&fd_master);
	max_fd = 0;

	for (std::vector<t_listen>::const_iterator it = listens.begin() ; it != listens.end() ; it++ )
	{
		//her bir server için ayrı bir Server nesnesi oluşturulur.
		Server		server(*it);//setUpSocket de burada çalışacak
		long		fd;//server içinde oluşturulacak socket fd'si

		fd = -1;
		//vector içinde gezip sırayla socket fd'lerini fd_master setine ,serverları da servers mapine ekleyecek.
		if ((fd = server.setUpSocket()) != -1)
		{
			// fd = server.getFd();
			FD_SET(fd, &fd_master);
			servers.insert(std::make_pair(fd, server));
			if (fd > max_fd)
				max_fd = fd;
			std::cout << GREEN << "Setting up " << it->host << ":" << it->port << "..." << RESET << std::endl;
			//socket oluşturuldu host:port
		}
		// else
		// {
		// 	break;
		// }
		// else
		// {
		// 	FD_CLR(fd, &fd_master);
		// 	close(server.getFd());
		// 	servers.erase(fd);
		// }
	}
	if (max_fd == 0)
	{
		std::cerr << RED << "Could not setup cluster !" << RESET << std::endl;
		return (-1);
	}
	return (0);
}

void	Cluster::select_section()
{
	//timeout 1 sn ayarlıyoruz. select() 1 sn içinde bir fd dönmezse select_return_value olacak
	this->timeout.tv_sec  = 1;
	this->timeout.tv_usec = 0;
	//ilk başta fd_master setindeki bütün socket fdleri reading set kümesine ekliyoruz.
	memcpy(&reading_set, &fd_master, sizeof(fd_master));
	//writing_set kullanmadan sıfırlıyoruz.
	FD_ZERO(&writing_set);
	for (std::vector<int>::iterator it = ready.begin() ; it != ready.end() ; it++)
		FD_SET(*it, &writing_set);
	//ready ilk başta boş, sonradan dolacak
	//ready içindeki (yani reading sette olup processe gidecek socketler) writing sete ekliyoruz.
	//socket send için hazır demektir
	std::cout << YELLOW << "\rWaiting on a connection..." << RESET << std::flush;
	//select işlevi kullanılarak takip edilen soketler için okuma ve yazma işlemleri için hazırlık yapılır. 
	//Bu döngüde, select işlevi sonucu select_return_value değişkenine değer atanana kadar çalışmaya devam eder.
	//timeout olursa sıfır döner. select_return_value'e değer atanana kadar while döner.
	this->select_return_value = select(max_fd + 1, &reading_set, &writing_set, NULL, &timeout);
}

void	Cluster::send_section()
{
	//socketten response gönderilecek
	//(writing_set) içinde bulunan soketler için döngü oluşturulur.
	// Bu soketler, ready vektöründe bulunur.
	for (std::vector<int>::iterator it = ready.begin() ; this->select_return_value && it != ready.end() ; it++)
	{
		if (FD_ISSET(*it, &writing_set))//Eğer soket, (writing_set) içinde bulunuyorsa, bunu select_section'da eklemiştik.
		{
			long	ret2 = sockets[*it]->send(*it);//send işlevi kullanılarak veriler(response) gönderilir. 

			if (ret2 == 0)//Gönderme işlemi başarılı olursa, o soket ready vektöründen silinir.
				ready.erase(it);
			else if (ret2 == -1)
			{
				//Eğer sokete yazma işlemi yaparken bir hata oluşursa, 
				//ilgili soket silinir ve sockets haritasından çıkarılır.
				FD_CLR(*it, &fd_master);
				FD_CLR(*it, &reading_set);
				sockets.erase(*it);
				ready.erase(it);
			}
			select_return_value = 0;
			break;
		}
	}
}

void	Cluster::recv_section()
{
	//server, client_fd'den gelen verileri alıyor.
	for (std::map<long, Server *>::iterator it = sockets.begin() ; this->select_return_value && it != sockets.end() ; it++)
	{
		long	client_fd = it->first;

		if (FD_ISSET(client_fd, &reading_set))
		{
			long	ret = it->second->recv(client_fd);
			//0: recv() fonksiyonu, bağlantı kapatıldığında 0 değerini döndürür. 
			//Bu durumda, alınacak veri kalmamıştır ve fonksiyon çağrısı sonlanır.
			if (ret == 0)
			{
				//request ayrıştırılması, işlenmesi ve response oluşturulması için gerekli işlemler yapılır.
				it->second->process(client_fd, this->httpScope);
 				//client_fd ready map'ine eklenir. Daha sonra writing_set eklenmek ve response gönderilmek üzere.
				ready.push_back(client_fd);
			}
			else if (ret == -1)
			{
			//hata durumunda socket sockets map'inden silinir
			//client_fd kayıtlı fd setlerinden çıkarılır.
			//iterator sockets mapin başına çekilir
				FD_CLR(client_fd, &fd_master);
				FD_CLR(client_fd, &reading_set);
				sockets.erase(client_fd);
				it = sockets.begin();
			}
			select_return_value = 0;
			break;
		}
	}
}

void 	Cluster::accept_section()
{
	//Server'ın socket_fd'sini kullanarak accept() fonksiyonundan yeni bir client_fd dönüyoruz.
	for (std::map<long, Server>::iterator it = servers.begin() ; this->select_return_value && it != servers.end() ; it++)
	{
		long	fd = it->first;
		if (FD_ISSET(fd, &reading_set))
		{
			//eğer server socket fd reading set içindeyse yeni bir socket(yeni bir client fd alınır) oluşturulur 
			//ve client'a hizmet etmek üzere hazır olduğunu belirtir
			//accept oluşturulan socketin fd'sini döner.
			long	client_fd = it->second.accept();//client_fd oluşturuldu. server bu fd'den gelecek verileri kabul edecek.
			if (client_fd != -1)
			{
				//client_fd fd_master kümesine eklenir
				//soketin gelen veri için bekleyen soketler arasında olduğunu belirler ve ana döngüde işlenmesine olanak tanır.
				FD_SET(client_fd, &fd_master);
				sockets.insert(std::make_pair(client_fd, &(it->second)));
				//veri kabul edilecek soketler arasına eklenir.
				if (client_fd > max_fd)
					max_fd = client_fd;
			}
			this->select_return_value = 0;
			break;
		}
	}
}

void	Cluster::run()
{
	while (1)
	{
		this->select_return_value = 0;
		while (select_return_value == 0)
			select_section();
		//select()'ten select_return_value'e değer atandıysa. Yani okuma ya da yazma için bir fd atandı.
		if (select_return_value > 0)
		{
			//ilk başta ready vector'ü boş olduğu için burayı es geçecek.
			if (ready.size() != 0)
				send_section();
			//ilk başta sockets map'i boş olduğu için burayı da es geçecek.
			else if (sockets.size() != 0)
                recv_section();
			else if (servers.size() != 0)
				accept_section();	
		}
		else
		{
			std::cerr << "Problem with select !" << std::endl;
			for (std::map<long, Server *>::iterator it = sockets.begin() ; it != sockets.end() ; it++)
				it->second->close(it->first);
			sockets.clear();
			ready.clear();
			FD_ZERO(&fd_master);
			for (std::map<long, Server>::iterator it = servers.begin() ; it != servers.end() ; it++)
				FD_SET(it->first, &fd_master);
		}
	}
}

void	Cluster::cleanServers()
{
	for ( std::map<long, Server>::iterator it = servers.begin() ; it != servers.end() ; it++ )
		it->second.clean();
}
void	Cluster::cleanSockets()
{
	for (std::map<long, Server *>::iterator it = sockets.begin() ; it != sockets.end() ; it++)
		it->second->close(it->first);
	sockets.clear();
}
void	Cluster::cleanReady()
{
	for (std::vector<int>::iterator it = ready.begin() ; it != ready.end() ; it++)
		close(*it);
	ready.clear();
}

void	Cluster::cleanAll()
{
	this->cleanServers();
	this->cleanSockets();
	this->cleanReady();
	FD_ZERO(&fd_master);
	FD_ZERO(&writing_set);
	FD_ZERO(&reading_set);

}




