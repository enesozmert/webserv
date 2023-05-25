#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "ErrorPage.hpp"
#include "CgiScope.hpp"
#include "RedirectionUri.hpp"
#include "../database/DataBase.hpp"
#include "IScope.hpp"
#include "Variable.hpp"
class ErrorPage;
class RedirectionUri;
class LocationScope : public IScope, public CgiScope
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
        bool longestMatch;//fatma ekledi
        //bunu true/false nasıl başlatacağımıza karar vermeliyiz
        //en uzun eşleşme açık olsun mu olmasın mı?
        // bool regex;
        //regex olsun mu olamasın mı karar vermeliyiz?

    public:
        LocationScope(/* args */);
        ~LocationScope();
        LocationScope(const LocationScope &location);
        LocationScope &operator=(const LocationScope &location);
        std::string getRoot();
        std::string getAutoindex();
        std::vector<std::string> getIndex();
        std::vector<std::string> getAllowMethods();
        ErrorPage getErrorPage();
        RedirectionUri getRedirectionUri();
        std::string getPath();
        std::string getClientBodyBufferSize();
        DataBase<Variable<std::string> > getKeywordDataBase();

        void setRoot(std::string root);
        void setIndex(std::string index);
        void setAutoindex(std::string autoIndex);
        void setAllowMethods(std::string allowMethods);
        void setErrorPage(ErrorPage errorPage);
        void setRedirectionUri(RedirectionUri redirectionUri);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);
        void setPath(std::string path);

        void keywordFill();
        bool usesLongestMatch() const;//fatma

        std::string getName() const;
        LocationScope* clone() const;
        LocationScope* cloneNew() const;
        
};