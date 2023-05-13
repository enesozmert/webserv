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
class LocationScope : public IScope
{
    private:
        std::string _path;
        std::string _root;
        std::string _autoindex;
        std::string _index;
        std::string _allowMethods;
        std::string _clientBodyBufferSize;
        std::vector<ErrorPage *> _errorPage;
        RedirectionUri _redirectionUri;
        DataBase<Variable<std::string> > _keywordDatabase;
        std::string _errorPageString;
        size_t _errorPageCount;

    public:
        LocationScope(/* args */);
        ~LocationScope();
        LocationScope(const LocationScope &location);
        LocationScope &operator=(const LocationScope &location);

        std::string getRoot();
        std::string getAutoindex();
        std::string getIndex();
        std::vector<std::string> getAllowMethods();
        std::vector<ErrorPage *> getErrorPage();
        std::string getErrorPageString();
        std::string getPath();
        RedirectionUri getRedirectionUri();
        DataBase<Variable<std::string> > getKeywordDataBase();
        size_t getErrorPageCount();

        void setRoot(std::string root);
        void setIndex(std::string index);
        void setAutoindex(std::string autoIndex);
        void setAllowMethods(std::string allowMethods);
        void setErrorPage(ErrorPage &errorPage);
        void setErrorPageString(std::string errorPageString);
        void setRedirectionUri(RedirectionUri redirectionUri);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);
        void setPath(std::string path);
        void setErrorPageCount(size_t errorPageCount);

        void keywordFill();

        std::string getName() const;
        LocationScope* clone() const;
        LocationScope* cloneNew() const;
        
};