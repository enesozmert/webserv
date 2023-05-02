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


std::string ErrorPage::getName() const
{
    return (this->name);
}


ErrorPage* ErrorPage::clone() const { return new ErrorPage(*this); }
