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
#include <fcntl.h>

#include "../entity/CgiVariable.hpp"
#include "../entity/HttpScope.hpp"
#include "../utils/Utils.hpp"
#include <fcntl.h>
#include <unistd.h>

class Cgi
{
    private:
        DataBase<CgiVariable<std::string, std::string> >    _envDatabase;
    	std::string							_body;
        std::map<std::string, std::string>  _query;
        std::string _methodName;
        std::string _path;
        int _port;
        std::string _contentLenght;
        std::string _contentType;
        //char	**env;
        //int request_body_pipe[2];
	    //int cgi_result_pipe[2];

    
    public:
        //Cgi();
        //Cgi(const Cgi &cgi);
        //Cgi& operator=(const Cgi &cgi);
    	Cgi(std::string methodName, std::string body, std::string path, int port);
        //~Cgi();
    
    	std::string		executeCgi(std::string scriptName);	// executes cgi and returns body
        DataBase<CgiVariable<std::string, std::string> > getEnvDataBase();
        void setEnvDatabase(DataBase<CgiVariable<std::string, std::string> > envDatabase);
        void keywordFill();
};