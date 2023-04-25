#include "../inc/entity/RedirectionUri.hpp"

RedirectionUri::RedirectionUri()
{
}

RedirectionUri::~RedirectionUri()
{
}

std::vector<std::string> RedirectionUri::getCodes()
{
    return (this->_codes);
}

std::string RedirectionUri::getPageName()
{
    return (this->_pageName);
}

void RedirectionUri::setCodes(std::string code)
{
    this->_codes.push_back(code);
}

void RedirectionUri::setPageName(std::string pageName)
{
    this->_pageName = pageName;
}
