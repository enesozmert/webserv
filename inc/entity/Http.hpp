#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "Server.hpp"
#include "ErrorPage.hpp"
#include "IScope.hpp"
#include "Variable.hpp"

class Server;
class ErrorPage;
class Http : public IScope
{
    private:
        std::string _clientMaxBodySize;
        std::vector<Server> _servers;
        ErrorPage _errorPage;
        DataBase<Variable<std::string> > _keywordDatabase;

    public:
        Http(/* args */);
        Http(char *file);
        ~Http();
        

        std::string getClientMaxBodySize();
        std::vector<Server> getServer();
        ErrorPage getErrorPage();
        DataBase<Variable<std::string> > getKeywordDataBase();

        void setClientMaxBodySize(std::string clientMaxBodySize);
        void setServer(Server server);
        void setErrorPage(ErrorPage errorPage);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);


        void keywordFill();

        std::string getName() const;
        Http* clone() const;

};