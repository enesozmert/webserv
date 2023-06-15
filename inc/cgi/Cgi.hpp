#pragma once

# define CGI_BUFSIZE 4096

#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "../entity/CgiVariable.hpp"
#include "../entity/HttpScope.hpp"
#include "../entity/Request.hpp"
#include "../entity/Response.hpp"
#include "../utils/Utils.hpp"
#include <fcntl.h>
#include <unistd.h>

class Request;
class Response;
class ServerScope;
class LocationScope;
class Cgi
{
    private:
        Request *_request;
        Response *_response;
        ServerScope *_serverScope;
        LocationScope *_locationScope;
        DataBase<CgiVariable<std::string, std::string> > _envDatabase;
    	std::string							_body;
        std::map<std::string, std::string>  _query;
        char	**env;
    
    public:
        Cgi();
        Cgi(const Cgi &cgi);
        Cgi& operator=(const Cgi &cgi);
    	Cgi(Request *request, Response *response,  ServerScope *serverScope, LocationScope *locationScope);
        ~Cgi();
    
    	std::string		executeCgi(std::string scriptName);
        DataBase<CgiVariable<std::string, std::string> > getEnvDataBase();
        void setEnvDatabase(DataBase<CgiVariable<std::string, std::string> > envDatabase);
        void keywordFill();
};