#include "../inc/entity/RedirectionUri.hpp"

RedirectionUri::RedirectionUri()
{
}

RedirectionUri::~RedirectionUri()
{
}

std::vector<std::string> RedirectionUri::getCodes() const
{
    return (this->_codes);
}

std::string RedirectionUri::getPageName() const
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


void RedirectionUri::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}


std::string RedirectionUri::getName() const
{
    return (this->name);
}


void RedirectionUri::keywordFill()
{
    std::vector<Variable<std::string> >  datas;

    //datas.push_back(Variable<std::string>("codes", &this->_codes));
    datas.push_back(Variable<std::string>("pageName", &this->_pageName));

    this->setKeywordDatabase(datas);
}

RedirectionUri* RedirectionUri::clone() const { return new RedirectionUri(*this); }
