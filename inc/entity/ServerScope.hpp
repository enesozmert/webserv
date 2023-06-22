#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "ErrorPage.hpp"
#include "LocationScope.hpp"
#include "IScope.hpp"
#include "Variable.hpp"
#include "CgiScope.hpp"
#include "Listen.hpp"
#include "../utils/Utils.hpp"


class ErrorPage;
class LocationScope;
class ServerScope : public IScope, public CgiScope
{
    private:
        t_listen                            listen;
        std::vector<LocationScope *>        _locations;
        std::string                         _port;
        std::string                         _ip;
        std::string                         _host;
        std::string                         _serverNames;
        std::string                         _root;
        std::string                         _index;
        std::string                         _listen;
        bool                                _isServerNameNothing;
        ErrorPage                           _errorPage;
        DataBase<Variable<std::string> >    _keywordDatabase;

    public:
        ServerScope();
        ~ServerScope();
        ServerScope(const ServerScope &server);
        ServerScope	&operator=(const ServerScope &server);

        void    setPort(std::string port);
        void    setIp(std::string ip);
        void    setHost(std::string host);
        void    setErrorPage(ErrorPage errorPage);
        void    setServerName(std::string serverName);
        void    setRoot(std::string root);
        void    setIndex(std::string index);
        void    setListen();
        void    setLocation(LocationScope *location);
        void    setIsServerNameNothing(bool isServerNameNothing);
        void    setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);

        std::string                         getPort();
        std::string                         getIp();
        std::string                         getHost();
        ErrorPage                           getErrorPage();
        std::string                         getRoot();
        t_listen                            getListen();
        bool                                getIsServerNameNothing();
        std::vector<std::string>            getServerName();
        std::vector<std::string>            getIndex();
        std::vector<LocationScope *>        getLocations();
        DataBase<Variable<std::string> >    getKeywordDataBase();
        
        void                                keywordFill();

        ServerScope*                        cloneNew() const;
        ServerScope*                        clone() const;
        std::string                         getName() const;
};

