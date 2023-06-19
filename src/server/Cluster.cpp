#include "../inc/server/Cluster.hpp"

Cluster::Cluster() {}

Cluster::~Cluster() {}

Cluster::Cluster(const Cluster &cluster)
{
		if (this != &cluster) {
		*this = cluster;
	}
}

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
	this->postLen = cluster.postLen;
	return (*this);
}

int Cluster::setUpCluster(HttpScope *http)
{
	this->loopControl = 1;
	this->httpScope = http;
	std::vector<t_listen> listens;
	listens = this->httpScope->getListens();
	this->status = 0;
	this->isMulti = 0;
	this->isFav = 0;
	this->postLen = 0;

	FD_ZERO(&this->writeFds);
	FD_ZERO(&this->readFds);
	FD_ZERO(&this->supReadFds);
	FD_ZERO(&this->supWriteFds);
	max_fd = 0;
	this->selected = 0;
	for (std::vector<t_listen>::const_iterator it = listens.begin(); it != listens.end(); it++)
	{
		int socket = 0;
		Server *server = new Server(this->httpScope, it->host, it->port);
		socket = server->getFd();
		FD_SET(socket, &readFds);
		servers.insert(std::pair<int, Server *>(socket, server));
		this->max_fd = socket + 1;
	}
	if (max_fd == 0)
	{
		std::cerr << RED << "Could not setup cluster !" << RESET << std::endl;
		return (-1);
	}
	return (0);
}

void Cluster::send_section()
{
	long sent;
	for (std::map<int, Client *>::iterator it = clients.begin(); this->loopControl && it != clients.end(); it++)
	{
		if (FD_ISSET(it->first, &supWriteFds))
		{
			std::string _response;
			_response = it->second->process();
			if(_response != "")
			{
				sent = send(it->first, _response.c_str(), _response.length(), 0);
				if (sent > 0)
				{
					std::cout << CYAN << "Send SuccesFully!" << RESET << std::endl << std::flush;
					close_connection(it);
				}
				else
					close_connection(it);
			}
			else{
				close_connection(it);
			}
			this->loopControl = 0;
			break;
		}
	}
}

void Cluster::recv_section()
{
	int	ret;
	char buffer[4096] = {0};
	size_t	sup_len = 0;

	ret = -1;
	for (std::map<int, Client *>::iterator it = clients.begin(); this->loopControl && it != clients.end(); it++)
	{
		if (FD_ISSET(it->first, &supReadFds))
		{
    		std::cout << YELLOW <<  "\nReceiving..." << RESET << std::endl;
			ret = recv(it->first, buffer, 4095, 0);
			std::cout << YELLOW <<  "ret..." << ret << RESET << std::endl;
			if (ret > 0)
			{
				std::cout << YELLOW << "this->status " << this->status << RESET << std::endl;
				std::cout << YELLOW << "this->multi " << this->isMulti << RESET << std::endl;
				std::cout << YELLOW << "this->isFav " << this->isFav << RESET << std::endl;
				if (this->status == 0)
				{
					it->second->setParserRequest(buffer);
					this->status = it->second->getStatus();
					this->isMulti = it->second->getMulti();
					this->isFav = it->second->getIsFav();
					this->postLen = it->second->getPostLen();
					std::cout << CYAN << "this->status " << this->status << RESET << std::endl;
					std::cout << CYAN << "this->multi " << this->isMulti << RESET << std::endl;
					std::cout << CYAN << "this->isFav " << this->isFav << RESET << std::endl;
					std::cout << CYAN << "this->postLen " << this->postLen << RESET << std::endl;
					if (this->isFav == 1)
					{
						std::string _body;
						int i = 0;
						std::string fav;
						std::string path = removeAdjacentSlashes(getPwd() + "/" + "index/icon.png");
						_body = openNread(path);
						fav = add_headers_favicon(_body);
						i = send(it->first, fav.c_str(), fav.length(), 0);
						if (i > 0)
						{
							std::cout << CYAN << "Send SuccesFully!" << RESET << std::endl << std::flush;
							std::cout << GREEN << "\rResponse :                " << std::endl << "[" << fav.substr(0, 500) << "..." << RESET << "]\n" << std::endl; 
						}
						FD_CLR(it->first, &this->readFds);
						close_connection(it);
						this->loopControl = 0;
						this->isFav = 0;
						memset(buffer, 0, sizeof(buffer));
						break;
					}
					if (this->status == 1)
					{
						if (this->isMulti == 1)
						{
							if (this->postLen == it->second->getPostLen())
							{
								FD_SET(it->first, &this->writeFds);
							}
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
					sup_len = it->second->postVal.length();
					if (!(sup_len + ret == std::string::npos))
					{
						it->second->postVal += std::string(buffer, ret);
						std::cout << "this->postLen" << this->postLen << std::endl;
						if (this->postLen == it->second->postVal.length())
						{
							FD_SET(it->first, &this->writeFds);
						}
					}
					else
					{
						std::cout << "So Big!" << std::endl << std::flush;
						close_connection(it);
					}
				}
			}
			else
			{
				if (ret == 0)
            		std::cout << RED << "\rConnection was closed by client.\n" << RESET << std::endl;
        		else if (ret == -1)
            		std::cout << RED << "\rRead error, closing connection.\n" << RESET << std::endl;
				close_connection(it);
				std::cout << "Connection Closed!" << std::endl << std::flush;
			}
			this->loopControl = 0;
			break;
		}
	}
}

void Cluster::accept_section()
{
	int client_fd;
	Client *client;
	for (std::map<int, Server *>::iterator it = servers.begin(); this->loopControl && it != servers.end(); it++)
	{
		if (FD_ISSET(it->first, &supReadFds))
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
				FD_SET(client_fd, &readFds);
				client = new Client(it->second);
				this->clients.insert(std::pair<int, Client *>(client_fd, client));
				findMaxFd();
			}
			this->loopControl = 0;
			break;
		}
	}
}

void Cluster::run()
{
	while (1)
	{
		this->selected = 0;
		while (selected == 0)
		{
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

