#include "../inc/server/Cluster.hpp"

Cluster::Cluster()
{
	FD_ZERO(&fdMaster);
	FD_ZERO(&writingSet);
	FD_ZERO(&readingSet);
}

Cluster::~Cluster()
{
	cleanAll();
}

Cluster::Cluster(const Cluster &cluster)
{
	*this = cluster;
}

int Cluster::setUpCluster(HttpScope *http)
{
	this->httpScope = http;
	// serverların hepsini vector olarak tutuyorduk. Onları çektik.
	std::vector<t_listen> listens;
	listens = this->httpScope->getListens();

	// olası hataları önlemek için bütün fd'ler 0'a eşitlenir.
	FD_ZERO(&fdMaster);
	maxFd = 0;

	for (std::vector<t_listen>::const_iterator it = listens.begin(); it != listens.end(); it++)
	{
		// her bir server için ayrı bir Server nesnesi oluşturulur.
		Server server(*it); // setUpSocket de burada çalışacak
		long fd;			// server içinde oluşturulacak socket fd'si

		fd = -1;
		// vector içinde gezip sırayla socket fd'lerini fdMaster setine ,serverları da servers mapine ekleyecek.
		if (server.setUpSocket() != -1)
		{
			fd = server.getFd();
			FD_SET(fd, &fdMaster);
			servers.insert(std::make_pair(fd, server));
			if (fd > maxFd)
				maxFd = fd;
			std::cout << GREEN << "Setting up " << it->host << ":" << it->port << "..." << RESET << std::endl;
			// socket oluşturuldu host:port
		}
	}
	if (maxFd == 0)
	{
		std::cerr << RED << "Could not setup cluster !" << RESET << std::endl;
		return (-1);
	}
	return (0);
}

void Cluster::select_section()
{
	// timeout 1 sn ayarlıyoruz. select() 1 sn içinde bir fd dönmezse selectReturnValue olacak
	this->timeout.tv_sec = 1;
	this->timeout.tv_usec = 0;
	// ilk başta fdMaster setindeki bütün socket fdleri reading set kümesine ekliyoruz.
	memcpy(&readingSet, &fdMaster, sizeof(fdMaster));
	// writingSet kullanmadan sıfırlıyoruz.
	FD_ZERO(&writingSet);
	for (std::vector<int>::iterator it = ready.begin(); it != ready.end(); it++)
		FD_SET(*it, &writingSet);
	// ready ilk başta boş, sonradan dolacak
	// ready içindeki (yani reading sette olup processe gidecek socketler) writing sete ekliyoruz.
	// socket send için hazır demektir
	std::cout << YELLOW << "\rWaiting on a connection..." << RESET << std::flush;
	// select işlevi kullanılarak takip edilen soketler için okuma ve yazma işlemleri için hazırlık yapılır.
	// Bu döngüde, select işlevi sonucu selectReturnValue değişkenine değer atanana kadar çalışmaya devam eder.
	// timeout olursa sıfır döner. selectReturnValue'e değer atanana kadar while döner.
	this->selectReturnValue = select(maxFd + 1, &readingSet, &writingSet, NULL, &timeout);
}

void Cluster::send_section()
{
	// socketten response gönderilecek
	//(writingSet) içinde bulunan soketler için döngü oluşturulur.
	//  Bu soketler, ready vektöründe bulunur.
	for (std::vector<int>::iterator it = ready.begin(); this->selectReturnValue && it != ready.end(); it++)
	{
		if (FD_ISSET(*it, &writingSet)) // Eğer soket, (writingSet) içinde bulunuyorsa, bunu select_section'da eklemiştik.
		{
			long ret2 = sockets[*it]->send(*it); // send işlevi kullanılarak veriler(response) gönderilir.

			if (ret2 == 0) // Gönderme işlemi başarılı olursa, o soket ready vektöründen silinir.
				ready.erase(it);
			else if (ret2 == -1)
			{
				// Eğer sokete yazma işlemi yaparken bir hata oluşursa,
				// ilgili soket silinir ve sockets haritasından çıkarılır.
				FD_CLR(*it, &fdMaster);
				FD_CLR(*it, &readingSet);
				sockets.erase(*it);
				ready.erase(it);
			}
			selectReturnValue = 0;
			break;
		}
	}
}

void Cluster::recv_section()
{
	// server, client_fd'den gelen verileri alıyor.
	for (std::map<long, Server *>::iterator it = sockets.begin(); this->selectReturnValue && it != sockets.end(); it++)
	{
		long client_fd = it->first;

		if (FD_ISSET(client_fd, &readingSet))
		{
			long ret = it->second->recv(client_fd);
			// 0: recv() fonksiyonu, bağlantı kapatıldığında 0 değerini döndürür.
			// Bu durumda, alınacak veri kalmamıştır ve fonksiyon çağrısı sonlanır.
			if (ret == 0)
			{
				// request ayrıştırılması, işlenmesi ve response oluşturulması için gerekli işlemler yapılır.
				it->second->process(client_fd, this->httpScope);
				// client_fd ready map'ine eklenir. Daha sonra writingSet eklenmek ve response gönderilmek üzere.
				ready.push_back(client_fd);
			}
			else if (ret == -1)
			{
				// hata durumunda socket sockets map'inden silinir
				// client_fd kayıtlı fd setlerinden çıkarılır.
				// iterator sockets mapin başına çekilir
				FD_CLR(client_fd, &fdMaster);
				FD_CLR(client_fd, &readingSet);
				sockets.erase(client_fd);
				it = sockets.begin();
			}
			selectReturnValue = 0;
			break;
		}
	}
}

void Cluster::accept_section()
{
	// Server'ın socket_fd'sini kullanarak accept() fonksiyonundan yeni bir client_fd dönüyoruz.
	for (std::map<long, Server>::iterator it = servers.begin(); this->selectReturnValue && it != servers.end(); it++)
	{
		long fd = it->first;
		if (FD_ISSET(fd, &readingSet))
		{
			// eğer server socket fd reading set içindeyse yeni bir socket(yeni bir client fd alınır) oluşturulur
			// ve client'a hizmet etmek üzere hazır olduğunu belirtir
			// accept oluşturulan socketin fd'sini döner.
			long client_fd = it->second.accept(); // client_fd oluşturuldu. server bu fd'den gelecek verileri kabul edecek.
			if (client_fd != -1)
			{
				// client_fd fdMaster kümesine eklenir
				// soketin gelen veri için bekleyen soketler arasında olduğunu belirler ve ana döngüde işlenmesine olanak tanır.
				FD_SET(client_fd, &fdMaster);
				sockets.insert(std::make_pair(client_fd, &(it->second)));
				// veri kabul edilecek soketler arasına eklenir.
				if (client_fd > maxFd)
					maxFd = client_fd;
			}
			this->selectReturnValue = 0;
			break;
		}
	}
}

void Cluster::run()
{
	while (1)
	{
		this->selectReturnValue = 0;
		while (selectReturnValue == 0)
		{
			select_section();
		}
		if (selectReturnValue > 0)
		{
			if (ready.size() != 0)
				send_section();
			else if (sockets.size() != 0)
				recv_section();
			else if (servers.size() != 0)
				accept_section();
		}
		else
		{
			for (std::map<long, Server *>::iterator it = sockets.begin(); it != sockets.end(); it++)
				it->second->close(it->first);
			sockets.clear();
			ready.clear();
			FD_ZERO(&fdMaster);
			for (std::map<long, Server>::iterator it = servers.begin(); it != servers.end(); it++)
				FD_SET(it->first, &fdMaster);
		}
	}
}

void Cluster::cleanServers()
{
	for (std::map<long, Server>::iterator it = servers.begin(); it != servers.end(); it++)
		it->second.clean();
	servers.clear();
}
void Cluster::cleanSockets()
{
	for (std::map<long, Server *>::iterator it = sockets.begin(); it != sockets.end(); it++)
		it->second->close(it->first);
	sockets.clear();
}
void Cluster::cleanReady()
{
	for (std::vector<int>::iterator it = ready.begin(); it != ready.end(); it++)
		close(*it);
	ready.clear();
}

void Cluster::cleanAll()
{
	FD_ZERO(&fdMaster);
	FD_ZERO(&writingSet);
	FD_ZERO(&readingSet);
	this->cleanServers();
	this->cleanSockets();
	this->cleanReady();
}