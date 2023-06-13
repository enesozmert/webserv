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

class LocationScope : public IScope, public CgiScope, public ErrorPage, public RedirectionUri
{
    private:
        std::string                         _path;
        std::string                         _root;
        std::string                         _autoindex;
        std::string                         _index;
        std::string                         _allowMethods;
        std::string                         _clientBodyBufferSize;
        DataBase<Variable<std::string> >    _keywordDatabase;

    public:
        LocationScope();
        ~LocationScope();
        LocationScope(const LocationScope &location);
        LocationScope &operator=(const LocationScope &location);

        std::string                         getRoot();
        std::string                         getAutoindex();
        std::vector<std::string>            getIndex();
        std::vector<std::string>            getAllowMethods();
        std::string                         getPath();
        std::string                         getClientBodyBufferSize();
        DataBase<Variable<std::string> >    getKeywordDataBase();

        void    setRoot(std::string root);
        void    setIndex(std::string index);
        void    setAutoindex(std::string autoIndex);
        void    setAllowMethods(std::string allowMethods);
        void    setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);
        void    setPath(std::string path);

        void            keywordFill();
        LocationScope*  clone() const;
        LocationScope*  cloneNew() const;
        std::string     getName() const;
        
};