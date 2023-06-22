#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "../entity/ServerScope.hpp"
#include "../entity/ErrorPage.hpp"
#include "../entity/IScope.hpp"
#include "../entity/Variable.hpp"

class ServerScope;
class ErrorPage;
class HttpScope : public IScope
{
    private:
        std::string                         _clientMaxBodySize;
        std::vector<ServerScope *>          _servers;
        std::vector<t_listen>               _listens;
        ErrorPage                           _errorPage;
        DataBase<Variable<std::string> >    _keywordDatabase;

    public:
        HttpScope();
        ~HttpScope();
        HttpScope(const HttpScope &http);
        HttpScope &operator=(const HttpScope &http);

        std::string                         getClientMaxBodySize();
        std::vector<ServerScope *>          getServers();
        ErrorPage                           getErrorPage();
        DataBase<Variable<std::string> >    getKeywordDataBase();
        std::vector<t_listen>               getListens();

        void setClientMaxBodySize(std::string clientMaxBodySize);
        void setServer(ServerScope *server);
        void setErrorPage(ErrorPage errorPage);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);
        void setListens(ServerScope* server);

        void        keywordFill();
        void        writeListens(std::vector<t_listen> _listens);

        HttpScope*  cloneNew() const;
        HttpScope*  clone() const;
        std::string getName() const;

};