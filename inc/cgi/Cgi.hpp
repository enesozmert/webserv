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
class Cgi
{
    private:
        Request *_request;
        Response *_response;
        ServerScope *_serverScope;
        DataBase<CgiVariable<std::string, std::string> >    _envDatabase;
    	std::string							_body;//HTTP istek gövdesi
        std::map<std::string, std::string>  _query;
        char	**env;
    
    public:
        Cgi();
        Cgi(const Cgi &cgi);
        Cgi& operator=(const Cgi &cgi);
    	Cgi(Request *request, ServerScope* serverScope, Response* response); // sets up env according to the request
        ~Cgi();
    
    	std::string		executeCgi(std::string scriptName);	// executes cgi and returns body
        DataBase<CgiVariable<std::string, std::string> > getEnvDataBase();
        void setEnvDatabase(DataBase<CgiVariable<std::string, std::string> > envDatabase);
        void keywordFill();
        void upload();
};