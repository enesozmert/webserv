#include "Server.hpp"

Server::Server() {}

Server::Server(const t_listen & _listen)
{
	this->_listen = _listen;
	fd = -1;
	this->setAddr();
	this->setUpServer();
}

void		Server::setAddr(void)
{
	ft_memset((char *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(_listen.host);
	addr.sin_port = htons(_listen.port);
}

int		Server::setUpServer()
{
	fd = socket(AF_INET, SOCK_STREAM, 0);//AF_INET-->ipv4   SOCK_STREAM-->TCP
	if (fd == -1)
	{
		std::cerr << "Could not create server." << std::endl;
		return (-1);
	}
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)//ip:port (192.168.1.1:443) (host)ip ve port arasındaki bağlantıyı kurar
	{
		std::cerr << "Could not bind port " << _listen.port << "." << std::endl;
		return (-1);
	}
	if (listen(fd, 10) == -1)//aynı anda max 10 bağlantı kabul etmeye hazır
	{
		std::cerr << "Could not listen." << std::endl;
		return (-1);
	}
	return (0);
}

long		Server::accept(void)
{
	long	client_fd;

	client_fd = ::accept(fd, NULL, NULL);//client_fd üzerinden iletişim kurulabilir.
	if (client_fd == -1)
		std::cerr << "Could not create socket. " << std::endl;
	else
	{
		fcntl(client_fd, F_SETFL, O_NONBLOCK);//socket ayarlarını bloklanmamış olarak değiştiriyoruz.
		requests.insert(std::make_pair(client_fd, ""));
	}
	return (client_fd);
}

void	Server::process(long socket, Config &config)
{
	//RequestConfig	request_config;
	//http class?
	Response		response;

	//chunked neyse processChunked üzerinden işlem görecek
	if (requests[socket].find("Transfer-Encoding: chunked") != std::string::npos && requests[socket].find("Transfer-Encoding: chunked") < requests[socket].find("\r\n\r\n"))
		this->processChunk(socket);

	if (requests[socket].size() < 1000)
		std::cout << "\nRequest :" << std::endl << "[" << requests[socket] << "]" << std::endl;
	else
		std::cout << "\nRequest :" << std::endl << "[" << _requests[socket].substr(0, 1000) << "..." << requests[socket].substr(_requests[socket].size() - 10, 15) << "]" << std::endl;

	if (requests[socket] != "")
	{
		Request			request(requests[socket]);//aldığımız isteği parçalamak üzere Request class'a gönderiyoruz.

		//Config dosyasından veriler çekilecek, http class?
		response.call(request, <çekilen verilerin tutulduğu class>);//http class gelecek

		//socket,request olan map yapısının requestini siliyoruz
		requests.erase(socket);
		//requeste cevap oluşturup map içinde socket,response şeklinde tutuyoruz.
		requests.insert(std::make_pair(socket, response.getResponse()));
	}
}

//chunk olayını araştır???
void		Server::processChunk(long socket)
{
	std::string	head = _requests[socket].substr(0, _requests[socket].find("\r\n\r\n"));
	std::string	chunks = _requests[socket].substr(_requests[socket].find("\r\n\r\n") + 4, _requests[socket].size() - 1);
	std::string	subchunk = chunks.substr(0, 100);
	std::string	body = "";
	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;

	while (chunksize)
	{
		i = chunks.find("\r\n", i) + 2;
		body += chunks.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = chunks.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}

	_requests[socket] = head + "\r\n\r\n" + body + "\r\n\r\n";
}

int			Server::recv(long socket)
{
	char	buffer[RECV_SIZE] = {0};
	int		ret;

	ret = ::recv(socket, buffer, RECV_SIZE - 1, 0);//ret alınan veri boyutu

	if (ret == 0 || ret == -1)
	{
		this->close(socket);
		if (!ret)//ret 0 olması durumu
			std::cout << "\rConnection was closed by client.\n" << std::endl;
		else
			std::cout << "\rRead error, closing connection.\n" << std::endl;//ret -1 olması durumu
		return (-1);
	}

	requests[socket] += std::string(buffer);//bufferda tutulan alınmış veriyi requests[socket] değeri olarak ekliyoruz.

	size_t	i = requests[socket].find("\r\n\r\n");//"\\r\\n\\r\\n" (HTTP istek başlığı ve gövdesi arasındaki ayırıcı) bu konumun indeksi i
	if (i != std::string::npos)//std::string::npos olmayan bir i değişkeni, requests[socket] öğesinde "\\r\\n\\r\\n" ifadesinin bulunduğu konumu temsil eder
	{
		if (requests[socket].find("Content-Length: ") == std::string::npos)//"Content-Length: " bulunduysa
		{
			if (requests[socket].find("Transfer-Encoding: chunked") != std::string::npos)//"Transfer-Encoding: chunked" bulunmadıysa
			{
				if (checkEnd(requests[socket], "0\r\n\r\n") == 0)//bunu başka fonksiyonla değiştir?
					return (0);
				else
					return (1);
			}
			else
				return (0);
		}
		//content-length başladığı yere indeks döndürürsek oradan karşılık gelen değeri substr ile çekiyoruz.
		size_t	len = std::atoi(requests[socket].substr(requests[socket].find("Content-Length: ") + 16, 10).c_str());
		//i-->istek başlığı uzunluğu
		//len-->istek gövdesi uzunluğu
		//"\\r\\n\\r\\n"-->4 seperator bunları toplayıp isteğin hepsi elimize ulaştı mı kontrol ederiz
		if (requests[socket].size() >= len + i + 4)
			return (0);
		else
			return (1);
	}

	return (1);
}

int			Server::send(long socket)
{
	static std::map<long, size_t>	sent;

	if (sent.find(socket) == sent.end())//gönderilecek mesaj kalmadıysa, socket içi boşsa
		sent[socket] = 0;

	std::string	str = requests[socket].substr(sent[socket], RECV_SIZE);
	int	ret = ::send(socket, str.c_str(), str.size(), 0);

	if (ret == -1)
	{
		this->close(socket);
		sent[socket] = 0;
		return (-1);
	}
	else
	{
		sent[socket] += ret;//ret gönderilmiş veri bunu sent[socket] eklediğimizde aşağıda karşılaştırma yapabiliriz
		if (sent[socket] >= requests[socket].size())
		{
			requests.erase(socket);//socketten gönderilen mesajı sileriz
			sent[socket] = 0;
			return (0);
		}
		else
			return (1);
	}
}


long		Server::get_fd(void)
{
	return (fd);
}

void		Server::close(int socket)
{
	if (socket > 0)
		::close(socket);
	requests.erase(socket);
}


void		Server::clean()
{
	if (fd > 0)
		::close(fd);
	fd = -1;
}

Server::~Server()
{
}