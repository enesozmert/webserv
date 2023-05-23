#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "ErrorPage.hpp"
#include "LocationScope.hpp"
#include "IScope.hpp"
#include "Variable.hpp"
#include "Listen.hpp"//fatma ekledi
#include "../utils/Utils.hpp"//fatma ekledi


class ErrorPage;
class LocationScope;
class ServerScope : public IScope
{
    private:
        t_listen listen;//fatma ekledi
        std::vector<LocationScope *> _locations;
        std::string _port;
        std::string _ip;
        std::string _host;
        std::string _serverNames;
        std::string _root;
        std::string _index;
        std::string _cgi_pass;
        std::string _listen;

        bool _isServerNameNothing;
        ErrorPage _errorPage;
        DataBase<Variable<std::string> > _keywordDatabase;

    public:
        ServerScope(/* args */);
        ~ServerScope();
        ServerScope(const ServerScope &server);
        ServerScope	&operator=(const ServerScope &server);

        void setPort(std::string port);
        void setIp(std::string ip);
        void setHost(std::string host);
        void setErrorPage(ErrorPage errorPage);
        void setServerName(std::string serverName);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setCgi_pass(std::string cgi_pass);
        void setListen();//fatma ekledi

        void setLocation(LocationScope *location);
        void setIsServerNameNothing(bool isServerNameNothing);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);

        std::string getPort();
        std::string getIp();
        std::string getHost();
        ErrorPage getErrorPage();
        std::vector<std::string> getServerName();
        std::string getRoot();
        std::string getIndex();
        std::string getCgi_pass();

        std::vector<LocationScope *> getLocations();
        bool getIsServerNameNothing();
        DataBase<Variable<std::string> > getKeywordDataBase();

        void keywordFill();

        t_listen    getListen();//fatma ekledi
        
        std::string getName() const;
        ServerScope* cloneNew() const;
        ServerScope* clone() const;


};

