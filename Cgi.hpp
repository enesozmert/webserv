#ifndef CGI_HPP
# define CGI_HPP

#include "webserv.hpp"

class Cgi
{
private:
	std::map<std::string, std::string>	_env;// CGI betiği tarafından kullanılabilen çevre değişkenlerini içeren bir map
	std::string							_body;//HTTP istek gövdesi
  
public:
	Cgi(Request &request, Config &config); // sets up env according to the request
	
	std::string		executeCgi(const std::string &scriptName);	// executes cgi and returns body
};


#endif