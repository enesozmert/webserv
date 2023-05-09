#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "ErrorPage.hpp"
#include "RedirectionUri.hpp"
#include "../database/DataBase.hpp"
#include "IScope.hpp"
#include "Variable.hpp"

class ErrorPage;
class RedirectionUri;
class Location : public IScope
{
    private:
        std::string _path;
        std::string _root;
        std::string _autoindex;
        std::string _index;
        std::string _allowMethods;
        std::string _clientBodyBufferSize;
        ErrorPage _errorPage;
        RedirectionUri _redirectionUri;
        DataBase<Variable<std::string> > _keywordDatabase;

    public:
        Location(/* args */);
        ~Location();
        std::string getRoot();
        std::string getAutoindex();
        std::string getIndex();
        std::vector<std::string> getAllowMethods();
        ErrorPage getErrorPage();
        RedirectionUri getRedirectionUri();
        DataBase<Variable<std::string> > getKeywordDataBase();
        std::string getPath();

        void setRoot(std::string root);
        void setIndex(std::string index);
        void setAutoindex(std::string autoIndex);
        void setAllowMethods(std::string allowMethods);
        void setErrorPage(ErrorPage errorPage);
        void setRedirectionUri(RedirectionUri redirectionUri);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);
        void setPath(std::string path);

        void keywordFill();

        std::string getName() const;
        Location* clone() const;
        
};