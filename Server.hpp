#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Config.hpp"

class Server {
private:
    std::map<long, std::string>	requests;
	t_listen					_listen;//host and port
	long						fd;
	struct sockaddr_in			addr;//host ve port ayarlanacak

public:
    Server();
    Server(const t_listen &listen);
    ~Server();

    long	get_fd();

    int     setUpServer();
    void    setAddr();
    void    clean();
    void    close(int socket);

    long    accept();
    void	process(long socket, Config &config);//http class?
    void	processChunk(long socket);
    int		send(long socket);
    int		recv(long socket);

};


#endif

