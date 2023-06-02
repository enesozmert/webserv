#pragma once

# define CGI_BUFSIZE 65536

#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../entity/HttpScope.hpp"
#include "../entity/Request.hpp"
#include "../entity/Response.hpp"
#include "../utils/Utils.hpp"
;
class Request;
class Cgi
{
    private:
    	std::map<std::string, std::string>	_env;// CGI betiği tarafından kullanılabilen çevre değişkenlerini içeren bir map
    	std::string							_body;//HTTP istek gövdesi
    
    public:
        Cgi();
    	Cgi(Request *request, ServerScope* server, Response* response); // sets up env according to the request
        ~Cgi();
    
    	std::string		executeCgi(const std::string &scriptName);	// executes cgi and returns body
};