#include "../inc/server/Cluster.hpp"

Cluster::Cluster() {}

Cluster::~Cluster() {}


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
	this->contentLen = 0;
	this->_response = "";
	this->method = "";
	this->body = "";
	this->MultiBody = "";

	this->favicon = add_headers_favicon(openNread(getPwd() + "/" + "favicon/favicon.ico"));

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
	}
	if (max_fd == 0)
	{
		_clusterException.run(400);
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
			_response = it->second->process(this->MultiBody);
			sent = send(it->first, _response.c_str(), _response.length(), 0);
			if (sent > 0)
				std::cout << CYAN << "Send Successful!" << RESET << std::endl << std::flush;
			close_connection(it);
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
			if (ret > 0)
			{
				if (this->status == 0)
				{
					it->second->setParserRequest(buffer);
					this->status = it->second->getStatus();
					this->isMulti = it->second->getMulti();
					this->method = it->second->getMethod();
					this->isFav = it->second->getIsFav();
					this->contentLen = it->second->getContentLen();
					this->body = it->second->getBody();
					if (this->isFav == 1)
					{
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
							if (static_cast<size_t>(ret) >= this->contentLen)
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
						_clusterException.run(401);
					}
				}
				else if (this->status == 1 && this->isMulti == 1)
				{
					sup_len = this->MultiBody.length();
					if (!(sup_len + ret == std::string::npos))
					{
						this->MultiBody += std::string(buffer, ret);
						if (this->contentLen == this->MultiBody.length())
						{
							FD_CLR(it->first, &this->readFds);
							FD_SET(it->first, &this->writeFds);
						}
					}
					else
					{
						_clusterException.run(402);
						close_connection(it);
					}
				}
			}
			else
			{
				if (ret == 0)
				{
            		std::cout << RED << "\rConnection was closed by client.\n" << RESET << std::endl;
				}
        		else if (ret == -1)
				{
					_clusterException.run(403);
				}
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
				_clusterException.run(404);
        		return ;
    		}
			if (client_fd != -1)
			{
				FD_SET(client_fd, &readFds);
				client = new Client(it->second, this->httpScope);
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
	this->method = "";
	this->contentLen = 0;
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

