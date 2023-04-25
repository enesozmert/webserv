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
    public:
        Http(/* args */);
        ~Http();

        std::string getClientMaxBodySize();
        Server getServer();
        ErrorPage getErrorPage();

        void setClientMaxBodySize(std::string clientMaxBodySize);
        void setServer(Server server);
        void setErrorPage(ErrorPage errorPage);
};