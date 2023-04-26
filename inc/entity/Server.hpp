#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "ErrorPage.hpp"
#include "Location.hpp"


class ErrorPage;
class Location;
class Server
{
    private:
        Location _location; //location sinifi gelecek
        std::string _port;// listen(string)[port(8080)]
        std::string _host; // listen(string)[host:port(127.0.0.1:80)]
        std::vector<std::string> _serverNames;
        bool _isServerNameNothing;
        ErrorPage _errorPage;
        DataBase<std::string> _keywordDatabase;

    public:
        Server(/* args */);
        ~Server();

        void setPort(std::string port);
        void setHost(std::string host);
        void setErrorPage(ErrorPage errorPage);
        void setServerName(std::string serverName);
        void setLocation(Location location);
        void setIsServerNameNothing(bool isServerNameNothing);
        void setKeywordDatabase(DataBase<std::string> keywordDatabase);

        std::string getPort();
        std::string getHost();
        ErrorPage getErrorPage();
        std::vector<std::string> getServerName();
        Location getLocation();
        bool getIsServerNameNothing();
        DataBase<std::string> getKeywordDataBase();

        void keywordFill();

};

