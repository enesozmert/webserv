#include "../inc/server/Cluster.hpp"

Cluster::Cluster() {
}

Cluster::~Cluster()
{
	std::cout << YELLOW << "Cluster destruct" << RESET << std::endl;
	//cleanAll();
}

Cluster::Cluster(const Cluster &cluster)
{
		if (this != &cluster) {
		*this = cluster;
	}
}

Cluster & Cluster::operator=(const Cluster & cluster)
{
	httpScope = cluster.httpScope;
	servers = cluster.servers;
	sockets = cluster.sockets;
	ready = cluster.ready;
	fd_master = cluster.fd_master;
	max_fd = cluster.max_fd;
	return (*this);
}

int Cluster::setUpCluster(HttpScope *http)
{
	this->httpScope = http;
	// serverların hepsini vector olarak tutuyorduk. Onları çektik.
	std::vector<t_listen> listens;
	listens = this->httpScope->getListens();

	// olası hataları önlemek için bütün fd'ler 0'a eşitlenir.
	FD_ZERO(&fd_master);
	max_fd = 0;

	for (std::vector<t_listen>::const_iterator it = listens.begin(); it != listens.end(); it++)
	{
		// her bir server için ayrı bir Server nesnesi oluşturulur.
		Server server(*it); // setUpSocket de burada çalışacak
		long fd;			// server içinde oluşturulacak socket fd'si

		// vector içinde gezip sırayla socket fd'lerini fd_master setine ,serverları da servers mapine ekleyecek.
		if (server.setUpSocket() != -1)
		{
			fd = server.getFd();
			FD_SET(fd, &fd_master);
			servers.insert(std::make_pair(fd, server));
			if (fd > max_fd)
				max_fd = fd;
			std::cout << GREEN << "Setting up " << it->host << ":" << it->port << "..." << RESET << std::endl;
			// socket oluşturuldu host:port
		}
	}
	if (max_fd == 0)
	{
		std::cerr << RED << "Could not setup cluster !" << RESET << std::endl;
		return (-1);
	}
	return (0);
}

void Cluster::run()
{
	std::string	dot[3] = {".  ", ".. ", "..."};
	int			n = 0;

	while (1)
	{
		fd_set		reading_set;
		fd_set		writing_set;
		struct timeval	timeout;
		int ret = 0;

		while (ret == 0)
		{
			// timeout 1 sn ayarlıyoruz. select() 1 sn içinde bir fd dönmezse select_return_value 0 olacak
			timeout.tv_sec  = 1;
			timeout.tv_usec = 0;
			// ilk başta fd_master setindeki bütün socket fdleri reading set kümesine ekliyoruz.
			memcpy(&reading_set, &fd_master, sizeof(fd_master));
			// writing_set kullanmadan sıfırlıyoruz.
			FD_ZERO(&writing_set);
			for (std::vector<int>::iterator it = ready.begin() ; it != ready.end() ; it++)
				FD_SET(*it, &writing_set);
			// ready ilk başta boş, sonradan dolacak
			// ready içindeki (yani reading sette olup processe gidecek socketler) writing sete ekliyoruz.
			// socket send için hazır demektir

			std::cout << "\rWaiting on a connection" << dot[n++] << std::flush;
			if (n == 3)
				n = 0;

			// select işlevi kullanılarak takip edilen soketler için okuma ve yazma işlemleri için hazırlık yapılır.
			// Bu döngüde, select işlevi sonucu select_return_value değişkenine değer atanana kadar çalışmaya devam eder.
			// timeout olursa sıfır döner. select_return_value'e değer atanana kadar while döner.

			ret = select(max_fd + 1, &reading_set, &writing_set, NULL, &timeout);
		}
		if (ret > 0)
		{
			// socketten response gönderilecek
			//(writing_set) içinde bulunan soketler için döngü oluşturulur.
			//  Bu soketler, ready vektöründe bulunur.
			for (std::vector<int>::iterator it = ready.begin(); ret && it != ready.end(); it++)
			{
				if (FD_ISSET(*it, &writing_set)) // Eğer soket, (writing_set) içinde bulunuyorsa, bunu select_section'da eklemiştik.
				{
					long ret = sockets[*it]->send(*it); // send işlevi kullanılarak veriler(response) gönderilir.

					if (ret == 0) // Gönderme işlemi başarılı olursa, o soket ready vektöründen silinir.
					{
						ready.erase(it);
						FD_CLR(*it, &writing_set);
						//FD_CLR(*it, &reading_set);
					}
					else if (ret == -1)
					{
						// Eğer sokete yazma işlemi yaparken bir hata oluşursa,
						// ilgili soket silinir ve sockets haritasından çıkarılır.
						//FD_CLR(*it, &fd_master);
						//FD_CLR(*it, &reading_set);
						FD_CLR(*it, &writing_set);
						sockets.erase(*it);
						ready.erase(it);
					}
					ret = 0;
					break;
				}
			}

			// server, client_fd'den gelen verileri alıyor.
			for (std::map<long, Server *>::iterator it = sockets.begin(); ret && it != sockets.end(); it++)
			{
				long socket = it->first;

				if (FD_ISSET(socket, &reading_set))
				{
					long ret = it->second->recv(socket);
					// 0: recv() fonksiyonu, bağlantı kapatıldığında 0 değerini döndürür.
					// Bu durumda, alınacak veri kalmamıştır ve fonksiyon çağrısı sonlanır.
					if (ret == 0)
					{
						// request ayrıştırılması, işlenmesi ve response oluşturulması için gerekli işlemler yapılır.
						it->second->process(socket, this->httpScope);
						// client_fd ready map'ine eklenir. Daha sonra writing_set eklenmek ve response gönderilmek üzere.
						ready.push_back(socket);
						FD_CLR(socket, &reading_set);
					}
					else if (ret == -1)
					{
						// hata durumunda socket sockets map'inden silinir
						// client_fd kayıtlı fd setlerinden çıkarılır.
						// iterator sockets mapin başına çekilir
						//FD_CLR(socket, &fd_master);
						FD_CLR(socket, &reading_set);
						sockets.erase(socket);
						it = sockets.begin();
					}
					ret = 0;
					break;
				}
			}

			// Server'ın socket_fd'sini kullanarak accept() fonksiyonundan yeni bir client_fd dönüyoruz.
			for (std::map<long, Server>::iterator it = servers.begin(); ret && it != servers.end(); it++)
			{
				long fd = it->first;
				if (FD_ISSET(fd, &reading_set))
				{
					// eğer server socket fd reading set içindeyse yeni bir socket(yeni bir client fd alınır) oluşturulur
					// ve client'a hizmet etmek üzere hazır olduğunu belirtir
					// accept oluşturulan socketin fd'sini döner.
					long socket = it->second.accept(); // client_fd oluşturuldu. server bu fd'den gelecek verileri kabul edecek.
					if (socket != -1)
					{
						// client_fd fd_master kümesine eklenir
						// soketin gelen veri için bekleyen soketler arasında olduğunu belirler ve ana döngüde işlenmesine olanak tanır.
						FD_SET(socket, &fd_master);
						sockets.insert(std::make_pair(socket, &(it->second)));
						// veri kabul edilecek soketler arasına eklenir.
						if (socket > max_fd)
							max_fd = socket;
					}
					ret = 0;
					break;
				}
			}
		}
		else if (ret == 0)
		{
			std::cerr << RED << "\nSelect Timeout" << RESET << std::endl;
		}
		else if (ret == -1)
		{
			std::cerr << RED << "\nSelect Error code: " << errno << "\nError message: " << strerror(errno) << RESET << std::endl;
			for (std::map<long, Server *>::iterator it = sockets.begin(); it != sockets.end(); it++)
				it->second->close(it->first);
			sockets.clear();
			ready.clear();
			FD_ZERO(&fd_master);
			for (std::map<long, Server>::iterator it = servers.begin(); it != servers.end(); it++)
				FD_SET(it->first, &fd_master);
		}
		n = 0;
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
	FD_ZERO(&fd_master);
	this->cleanServers();
	this->cleanSockets();
	this->cleanReady();
}

