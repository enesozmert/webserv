#include "../inc/entity/RedirectionUri.hpp"

RedirectionUri::RedirectionUri()
{
}

RedirectionUri::~RedirectionUri()
{
}

RedirectionUri::RedirectionUri(const RedirectionUri &redirectionUri)
{
    *this = redirectionUri;
}

RedirectionUri	&RedirectionUri::operator=(const RedirectionUri &redirectionUri)
{
    if (this == &redirectionUri)
        return (*this);
    this->_codes = redirectionUri._codes;
    this->_pageName = redirectionUri._pageName;
    return (*this);
}

std::vector<std::string> RedirectionUri::getCodes() const
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

std::string RedirectionUri::getPageName() const
{
    return (this->_pageName);
}

void RedirectionUri::setCodes(std::string code)
{
    (void)code;
    // this->_codes.push_back(code);
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
    // std::vector<Variable<std::string> >  datas;

    _keywordDatabase.insertData(Variable<std::string>("codes", &this->_codes));
    _keywordDatabase.insertData(Variable<std::string>("pageName", &this->_pageName));

    // this->setKeywordDatabase(datas);
}

RedirectionUri* RedirectionUri::cloneNew() const { return new RedirectionUri(); }
RedirectionUri* RedirectionUri::clone() const { return new RedirectionUri(*this); }
