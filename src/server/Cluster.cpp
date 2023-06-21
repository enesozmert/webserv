#include "../inc/server/Cluster.hpp"

<<<<<<< HEAD
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
=======
Cluster::Cluster() {}

Cluster::~Cluster() {}
/*
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61

Cluster::Cluster(const Cluster &cluster)
{
		if (this != &cluster) {
		*this = cluster;
	}
}

<<<<<<< HEAD
=======
Cluster& Cluster::operator=(const Cluster & cluster)
{
	if (this == &cluster)
        return (*this);
	httpScope = cluster.httpScope;
	servers = cluster.servers;
	max_fd = cluster.max_fd;
	readFds = cluster.readFds;
	writeFds = cluster.writeFds;
	supReadFds = cluster.supReadFds;
	supWriteFds = cluster.supWriteFds;
	selected = cluster.selected;
	this->isMulti = cluster.isMulti;
	this->loopControl = cluster.loopControl;
	this->status = cluster.status;
	this->clients = cluster.clients;
	this->isFav = cluster.isFav;
	this->ContentLen = cluster.ContentLen;
	this->method = cluster.method;
	this->favicon = cluster.favicon;
	this->_response = cluster._response;
	this->body = cluster._body;
	return (*this);
} */

>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
int Cluster::setUpCluster(HttpScope *http)
{
	Server *server = NULL;
	int socket = -1;
	this->loopControl = 1;
	this->httpScope = http;
	std::vector<t_listen> listens;
	listens = this->httpScope->getListens();
	this->status = 0;
	this->isMulti = 0;
	this->isFav = 0;
	this->ContentLen = 0;
	this->_response = "";
	this->method = "";
	this->body = "";
	this->MultiBody = "";

	this->favicon = add_headers_favicon(openNread(getPwd() + "/" + "index/fv.png"));

<<<<<<< HEAD
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
=======
	FD_ZERO(&this->writeFds);
	FD_ZERO(&this->readFds);
	FD_ZERO(&this->supReadFds);
	FD_ZERO(&this->supWriteFds);
	max_fd = 0;
	this->selected = 0;

	for (std::vector<t_listen>::const_iterator it = listens.begin(); it != listens.end(); it++)
	{
		server = new Server(it->host, it->port);
		socket = server->getFd();
		FD_SET(socket, &readFds);
		servers.insert(std::pair<int, Server *>(socket, server));
		this->max_fd = socket + 1;
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
	}
	if (maxFd == 0)
	{
		std::cerr << RED << "Could not setup cluster !" << RESET << std::endl;
		return (-1);
	}
	return (0);
}

<<<<<<< HEAD
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
=======
void Cluster::send_section()
{
	long sent;
	for (std::map<int, Client *>::iterator it = clients.begin(); this->loopControl && it != clients.end(); it++)
	{
		if (FD_ISSET(it->first, &supWriteFds))
		{
			_response = it->second->process(this->MultiBody);
			sent = send(it->first, _response.c_str(), _response.length(), 0);
			if (sent > 0)
				std::cout << CYAN << "Send Successful!" << RESET << std::endl << std::flush;
			close_connection(it);
			this->loopControl = 0;
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
			break;
		}
	}
}

void Cluster::recv_section()
{
<<<<<<< HEAD
	// server, client_fd'den gelen verileri alıyor.
	for (std::map<long, Server *>::iterator it = sockets.begin(); this->selectReturnValue && it != sockets.end(); it++)
	{
		long client_fd = it->first;

		if (FD_ISSET(client_fd, &readingSet))
=======
	int	ret;
	char buffer[4096] = {0};
	size_t	sup_len = 0;

	ret = -1;
	for (std::map<int, Client *>::iterator it = clients.begin(); this->loopControl && it != clients.end(); it++)
	{
		if (FD_ISSET(it->first, &supReadFds))
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
		{
    		std::cout << YELLOW <<  "\nReceiving..." << RESET << std::endl;
			ret = recv(it->first, buffer, 4095, 0);
			std::cout << YELLOW <<  "ret..." << ret << RESET << std::endl;
			if (ret > 0)
			{
<<<<<<< HEAD
				// request ayrıştırılması, işlenmesi ve response oluşturulması için gerekli işlemler yapılır.
				it->second->process(client_fd, this->httpScope);
				// client_fd ready map'ine eklenir. Daha sonra writingSet eklenmek ve response gönderilmek üzere.
				ready.push_back(client_fd);
=======
				if (this->status == 0)
				{
					it->second->setParserRequest(buffer);
					this->status = it->second->getStatus();
					this->isMulti = it->second->getMulti();
					this->method = it->second->getMethod();
					this->isFav = it->second->getIsFav();
					this->ContentLen = it->second->getContentLen();
					this->body = it->second->getBody();
					if (this->isFav == 1)
					{
						std::cout << YELLOW << "favicon bulundu" << RESET << std::endl;
						if (send(it->first, this->favicon.c_str(), this->favicon.length(), 0) > 0)
							std::cout << CYAN << "Send Successful Favicon!" << RESET << std::endl << std::flush;
						this->loopControl = 0;
						close_connection(it);
						break ;
					}
					else if (this->status == 1)
					{
						if (this->isMulti == 1)
						{
							if (static_cast<size_t>(ret) >= this->ContentLen)
							{
								FD_CLR(it->first, &this->readFds);
								FD_SET(it->first, &this->writeFds);
								this->MultiBody += std::string(buffer, ret);
								this->MultiBody = this->MultiBody.substr(this->MultiBody.find("------Web"));
							}
							else if (this->body != "")
							{
								this->MultiBody += std::string(buffer, ret);
								this->MultiBody = this->MultiBody.substr(this->MultiBody.find("------Web"));
								std::cout << "this->MultiBody.length()body eklenince" << this->MultiBody.length() << std::endl;
							}
							this->body = "";
						}
						else
						{
							FD_CLR(it->first, &this->readFds);
							FD_SET(it->first, &this->writeFds);
						}
					}
					else
					{
						close_connection(it);
						std::cout << "Wrong Data!" << std::endl << std::flush;
					}
				}
				else if (this->status == 1 && this->isMulti == 1)
				{
					sup_len = this->MultiBody.length();
					std::cout << "sup_len" << sup_len << std::endl;
					if (!(sup_len + ret == std::string::npos))
					{
						this->MultiBody += std::string(buffer, ret);
						std::cout << "this->MultiBody.length()" << this->MultiBody.length() << std::endl;
						std::cout << "this->ContentLen" << this->ContentLen << std::endl;
						if (this->ContentLen == this->MultiBody.length())
						{
							FD_CLR(it->first, &this->readFds);
							FD_SET(it->first, &this->writeFds);
						}
					}
					else
					{
						std::cout << "So Big!" << std::endl << std::flush;
						close_connection(it);
					}
				}
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
			}
			else
			{
<<<<<<< HEAD
				// hata durumunda socket sockets map'inden silinir
				// client_fd kayıtlı fd setlerinden çıkarılır.
				// iterator sockets mapin başına çekilir
				FD_CLR(client_fd, &fdMaster);
				FD_CLR(client_fd, &readingSet);
				sockets.erase(client_fd);
				it = sockets.begin();
			}
			selectReturnValue = 0;
=======
				if (ret == 0)
            		std::cout << RED << "\rConnection was closed by client.\n" << RESET << std::endl;
        		else if (ret == -1)
            		std::cout << RED << "\rRead error, closing connection.\n" << RESET << std::endl;
				close_connection(it);
				std::cout << "Connection Closed!" << std::endl << std::flush;
			}
			this->loopControl = 0;
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
			break;
		}
	}
}

void Cluster::accept_section()
{
<<<<<<< HEAD
	// Server'ın socket_fd'sini kullanarak accept() fonksiyonundan yeni bir client_fd dönüyoruz.
	for (std::map<long, Server>::iterator it = servers.begin(); this->selectReturnValue && it != servers.end(); it++)
	{
		long fd = it->first;
		if (FD_ISSET(fd, &readingSet))
=======
	int client_fd;
	Client *client;
	for (std::map<int, Server *>::iterator it = servers.begin(); this->loopControl && it != servers.end(); it++)
	{
		if (FD_ISSET(it->first, &supReadFds))
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
		{
			std::cout << YELLOW << "\nAccepting..." << RESET << std::endl;
		
    		client_fd = ::accept(it->first, NULL, NULL);
    		if (client_fd == -1)
    		{
      			std::cerr << RED << "Could not create socket. " << RESET << std::endl;
        		return ;
    		}
			if (client_fd != -1)
			{
<<<<<<< HEAD
				// client_fd fdMaster kümesine eklenir
				// soketin gelen veri için bekleyen soketler arasında olduğunu belirler ve ana döngüde işlenmesine olanak tanır.
				FD_SET(client_fd, &fdMaster);
				sockets.insert(std::make_pair(client_fd, &(it->second)));
				// veri kabul edilecek soketler arasına eklenir.
				if (client_fd > maxFd)
					maxFd = client_fd;
			}
			this->selectReturnValue = 0;
=======
				FD_SET(client_fd, &readFds);
				client = new Client(it->second, this->httpScope);
				this->clients.insert(std::pair<int, Client *>(client_fd, client));
				findMaxFd();
			}
			this->loopControl = 0;
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
			break;
		}
	}
}

void Cluster::run()
{
	while (1)
	{
<<<<<<< HEAD
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
=======
		this->selected = 0;
		while (selected == 0)
		{
			FD_ZERO(&supReadFds);
			FD_ZERO(&supWriteFds);
			this->supWriteFds = this->writeFds;
			this->supReadFds = this->readFds;
			this->selected = select(max_fd, &this->supReadFds, &this->supWriteFds, NULL, NULL);
		}
		if (selected > 0)
		{
			this->loopControl = 1;
			recv_section();
			send_section();
			accept_section();
		}
		else
		{
			std::cerr << RED << "\nSelect Error code: " << errno << "\nError message: " << strerror(errno) << RESET << std::endl;
			cleanAll();
		}
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
	}
}

void Cluster::cleanServers()
{
	for (std::map<int, Server *>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		close(it->first);
		delete(it->second);
	}
	servers.clear();
}

void Cluster::cleanClients()
{
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		close(it->first);
		delete(it->second);
	}
	clients.clear();
}

void Cluster::cleanAll()
{
<<<<<<< HEAD
	FD_ZERO(&fdMaster);
	FD_ZERO(&writingSet);
	FD_ZERO(&readingSet);
	this->cleanServers();
	this->cleanSockets();
	this->cleanReady();
}
=======
	FD_ZERO(&readFds);
	FD_ZERO(&writeFds);
	FD_ZERO(&supReadFds);
	FD_ZERO(&supWriteFds);
	this->cleanServers();
	this->cleanClients();
}

void	Cluster::close_connection(std::map<int, Client *>::iterator it)
{
	this->status = 0;
	this->isMulti = 0;
	this->isFav = 0;
	this->method = "";
	this->ContentLen = 0;
	this->body = "";
	this->MultiBody = "";
	FD_CLR(it->first, &this->readFds);
	FD_CLR(it->first, &this->writeFds);
	close(it->first);
	delete(it->second);
	this->clients.erase(it);
	this->findMaxFd();
}

void	Cluster::findMaxFd()
{
	this->max_fd = -1;

	for(std::map<int, Server *>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		if (it->first >= this->max_fd)
		{
			this->max_fd = it->first;
			this->max_fd += 1;
		}
	}

	for(std::map<int, Client *>::iterator iter = this->clients.begin(); this->selected && iter != this->clients.end(); iter++)
	{
		if (iter->first >= this->max_fd)
		{
			this->max_fd = iter->first;
			this->max_fd += 1;
		}
	}
}

>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
