#ifndef WEBSERV_HPP
# define WEBSERV_HPP

typedef struct	s_listen {
	unsigned int	host;
	int			port;
}				t_listen;


// CPP Includes
# include <iostream>
# include <iomanip>
# include <sstream>
# include <fstream>
# include <string>
# include <limits>
# include <cstdio>

// CPP Containers
# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>
# include <list>
# include <utility>


// C Includes
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <time.h>
# include <limits.h>
# include <errno.h>
# include <dirent.h>

// C System
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>

// C Network
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <netinet/in.h>


#include "libft.h"
//The default buffer size of a pipeline (Unix) is 64KiB (65536 bytes)
# define RECV_SIZE 65536
# define CGI_BUFSIZE 65536

std::string					to_string(size_t n);
char	**maptoStr(std::map<std::string, std::string> map);
std::string trim(const std::string &s);
std::string	formatHeaderForCGI(std::string& key);


#endif