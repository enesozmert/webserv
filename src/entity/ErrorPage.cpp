#include "../inc/entity/ErrorPage.hpp"

ErrorPage::ErrorPage()
{
}

ErrorPage::~ErrorPage()
{
}

std::vector<std::string> ErrorPage::getCodes()
{
    return (this->_codes);
}

std::string ErrorPage::getPageName()
{
    return (this->_pageName);
}

void ErrorPage::setCodes(std::string code)
{
    this->_codes.push_back(code);
}

void ErrorPage::setPageName(std::string pageName)
{
    this->_pageName = pageName;
}

void ErrorPage::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

std::string ErrorPage::getName() const
{
    return (this->name);
}

void ErrorPage::keywordFill()
{
    std::vector<Variable<std::string> >  datas;

    //datas.push_back(Variable<std::string>("codes", &this->_codes));
    datas.push_back(Variable<std::string>("pageName", &this->_pageName));

    this->setKeywordDatabase(datas);
}


ErrorPage* ErrorPage::clone() const { return new ErrorPage(*this); }
