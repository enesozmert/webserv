#include "../inc/entity/HttpScope.hpp"

HttpScope::HttpScope(/* args */)
{
    this->keywordFill();

}

HttpScope::~HttpScope()
{
}

HttpScope::HttpScope(const HttpScope &http)
{
    *this = http;
}

HttpScope	&HttpScope::operator=(const HttpScope &http)
{
    if (this == &http)
        return (*this);
    this->_clientMaxBodySize = http._clientMaxBodySize;
    return (*this);
}

std::string HttpScope::getClientMaxBodySize()
{
    return (this->_clientMaxBodySize);
}
std::vector<ServerScope *> HttpScope::getServers()
{
    return (this->_servers);
}
ErrorPage HttpScope::getErrorPage()
{
    return (this->_errorPage);
}
DataBase<Variable<std::string> > HttpScope::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}

void HttpScope::setClientMaxBodySize(std::string clientMaxBodySize)
{
    this->_clientMaxBodySize = clientMaxBodySize;
}
void HttpScope::setServer(ServerScope *server)
{
    this->_servers.push_back(server);
}
void HttpScope::setErrorPage(ErrorPage errorPage)
{
    this->_errorPage = errorPage;
}
void HttpScope::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void HttpScope::keywordFill()
{
    _keywordDatabase.insertData(Variable<std::string>("client_max_body_size", &this->_clientMaxBodySize));
}

std::string HttpScope::getName() const
{
    return (this->name);
}

HttpScope* HttpScope::cloneNew() const { return new HttpScope(); }
HttpScope* HttpScope::clone() const { return new HttpScope(*this); }

