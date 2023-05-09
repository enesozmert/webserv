#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "IScope.hpp"
#include "../database/DataBase.hpp"
#include "Variable.hpp"



class RedirectionUri : public IScope
{
private:
    std::string _codes;
    std::string _pageName;
    DataBase<Variable<std::string> > _keywordDatabase;

public:
    RedirectionUri(/* args */);
    ~RedirectionUri();

    std::vector<std::string> getCodes() const;
    void setCodes(std::string code);
    void setPageName(std::string pageName);
    void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);


    std::string getName() const;
    std::string getPageName() const;
    DataBase<Variable<std::string> > getKeywordDataBase();

    void keywordFill();


    RedirectionUri* clone() const;

};