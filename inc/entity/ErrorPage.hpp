#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "IScope.hpp"
#include "Variable.hpp"
#include "../database/DataBase.hpp"


class ErrorPage : public IScope
{
    private:
        std::string _codes;
        std::string _pageName;
        DataBase<Variable<std::string> > _keywordDatabase;
        
    public:
        ErrorPage(/* args */);
        ~ErrorPage();
        ErrorPage(const ErrorPage &errorPage);
        ErrorPage &operator=(const ErrorPage &errorPage);
    
        std::vector<std::string> getCodes();
        std::string getPageName();
        std::string getName() const;
        DataBase<Variable<std::string> > getKeywordDataBase();


        void setCodes(std::string code);
        void setPageName(std::string pageName);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);

        void keywordFill();

        ErrorPage* cloneNew() const;
        ErrorPage* clone() const;
};