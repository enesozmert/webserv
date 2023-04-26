#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include "Server.hpp"
#include "ErrorPage.hpp"

class Server;
class ErrorPage;
class Http
{
    private:
        std::string _clientMaxBodySize;
        std::vector<Server> _servers;
        ErrorPage _errorpage;
    public:
        Http(/* args */);
        Http(char *file);
        ~Http();
        

        std::string getClientMaxBodySize();
        Server getServer();
        ErrorPage getErrorPage();

        void setClientMaxBodySize(std::string clientMaxBodySize);
        void setServer(Server server);
        void setErrorPage(ErrorPage errorPage);

        void parseConfig(char *file);
};