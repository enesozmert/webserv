#include "../inc/entity/ErrorPage.hpp"

ErrorPage::ErrorPage()
{
}

ErrorPage::~ErrorPage()
{
}

std::vector<std::string> ErrorPage::getCodes()
{
    std::vector<std::string> codes;
    std::string tempCodes;
    std::stringstream sp(this->_codes);

    while (sp << tempCodes)
    {
        codes.push_back(tempCodes);
    }
    return (codes);
}

std::string ErrorPage::getPageName()
{
    return (this->_pageName);
}

void ErrorPage::setCodes(std::string code)
{
    (void)code;
    // this->_codes.push_back(code);
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
    _keywordDatabase.insertData(Variable<std::string>("codes", &this->_codes));
    _keywordDatabase.insertData(Variable<std::string>("pageName", &this->_pageName));
}


ErrorPage* ErrorPage::clone() const { return new ErrorPage(); }
