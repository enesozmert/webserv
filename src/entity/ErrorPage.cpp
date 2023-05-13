#include "../inc/entity/ErrorPage.hpp"

ErrorPage::ErrorPage()
{
    this->name = "ErrorPage";
}

ErrorPage::~ErrorPage()
{
}

ErrorPage::ErrorPage(const ErrorPage &errorPage)
{
    *this = errorPage;
}

ErrorPage	&ErrorPage::operator=(const ErrorPage &errorPage)
{
    if (this == &errorPage)
        return (*this);
    this->_codes = errorPage._codes;
    this->_pageName = errorPage._pageName;
    return (*this);
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
    this->_codes = code;
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

void ErrorPage::fillErrorPage(std::string errorPageString)
{
    std::string splitErrorPage;
    std::string splitErrorPage2;
    std::istringstream s(errorPageString);

    s >> std::ws >> splitErrorPage;
    // std::getline(s >> std::ws, splitErrorPage);
    this->setCodes(splitErrorPage); //dışarıdan gelen indexe gore ilgili elemanın code ve page'ini doldursun
    s >> std::ws >> splitErrorPage2;
    // std::getline(s >> std::ws, splitErrorPage);
    this->setPageName(splitErrorPage2);
}


ErrorPage* ErrorPage::cloneNew() const { return new ErrorPage(); }
ErrorPage* ErrorPage::clone() const { return new ErrorPage(*this); }
