#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "ServerScope.hpp"
#include "ErrorPage.hpp"
#include "IScope.hpp"
#include "Variable.hpp"

class ServerScope;
class ErrorPage;
class HttpScope : public IScope
{
    private:
        std::string _clientMaxBodySize;
        std::vector<ServerScope *> _servers;
        ErrorPage _errorPage;
        DataBase<Variable<std::string> > _keywordDatabase;

    public:
        HttpScope(/* args */);
        ~HttpScope();
        HttpScope(const HttpScope &http);
        HttpScope &operator=(const HttpScope &http);

        std::string getClientMaxBodySize();
        std::vector<ServerScope *> getServers();
        ErrorPage getErrorPage();
        DataBase<Variable<std::string> > getKeywordDataBase();

        void setClientMaxBodySize(std::string clientMaxBodySize);
        void setServer(ServerScope *server);
        void setErrorPage(ErrorPage errorPage);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);

        void keywordFill();

        std::string getName() const;
        HttpScope* cloneNew() const;
        HttpScope* clone() const;

};