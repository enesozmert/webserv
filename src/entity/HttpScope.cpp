#include "../inc/entity/HttpScope.hpp"

HttpScope::HttpScope()
{
    this->keywordFill();
}

HttpScope::~HttpScope() {}

HttpScope::HttpScope(const HttpScope &http)
{
    *this = http;
}

HttpScope	&HttpScope::operator=(const HttpScope &http)
{
    if (this == &http)
        return (*this);
    this->_clientMaxBodySize = http._clientMaxBodySize;
    this->_errorPage = http._errorPage;
    this->_keywordDatabase = http._keywordDatabase;
    this->_listens = http._listens;
    this->_servers = http._servers;
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
    this->setListens(server);
}

void HttpScope::setErrorPage(ErrorPage errorPage)
{
    this->_errorPage = errorPage;
}

void HttpScope::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void HttpScope::setListens(ServerScope* server)
{
    this->_listens.push_back(server->getListen());
}

void HttpScope::keywordFill()
{
    _keywordDatabase.insertData(Variable<std::string>("client_max_body_size", &this->_clientMaxBodySize));
}

std::vector<t_listen> HttpScope::getListens()
{
    return _listens;
}

void HttpScope::writeListens(std::vector<t_listen> _listens)
{
    for(size_t i = 0; i < _listens.size(); i++)
    {
        std::cout << "listen host : " << _listens[i].host << std::endl;
        std::cout << "listen port : " << _listens[i].port << std::endl;
    }

}

HttpScope* HttpScope::cloneNew() const { return new HttpScope(); }
HttpScope* HttpScope::clone() const { return new HttpScope(*this); }
std::string HttpScope::getName() const { return (this->name); }