#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include "IScope.hpp"
#include "Variable.hpp"
#include "../database/DataBase.hpp"


class ErrorPage : public IScope
{
    private:
        std::vector<std::string> _codes;
        std::string _pageName;
        DataBase<Variable<std::string> > _keywordDatabase;
        
    public:
        ErrorPage(/* args */);
        ~ErrorPage();
    
        std::vector<std::string> getCodes();
        std::string getPageName();
        std::string getName() const;
        DataBase<Variable<std::string> > getKeywordDataBase();


        void setCodes(std::string code);
        void setPageName(std::string pageName);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);

        void keywordFill();

        ErrorPage* clone() const;
};