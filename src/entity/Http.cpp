#include "../inc/entity/Http.hpp"

Http::Http(/* args */)
{
    this->keywordFill();

}

Http::~Http()
{
}

Http::Http(const Http &http)
{
    *this = http;
}

Http	&Http::operator=(const Http &http)
{
    if (this == &http)
        return (*this);
    this->_clientMaxBodySize = http._clientMaxBodySize;
    return (*this);
}

std::string Http::getClientMaxBodySize()
{
    return (this->_clientMaxBodySize);
}
std::vector<Server *> Http::getServers()
{
    return (this->_servers);
}
ErrorPage Http::getErrorPage()
{
    return (this->_errorPage);
}
DataBase<Variable<std::string> > Http::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}

void Http::setClientMaxBodySize(std::string clientMaxBodySize)
{
    this->_clientMaxBodySize = clientMaxBodySize;
}
void Http::setServer(Server *server)
{
    this->_servers.push_back(server);
}
void Http::setErrorPage(ErrorPage errorPage)
{
    this->_errorPage = errorPage;
}
void Http::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void Http::keywordFill()
{
    _keywordDatabase.insertData(Variable<std::string>("client_max_body_size", &this->_clientMaxBodySize));
}

std::string Http::getName() const
{
    return (this->name);
}

Http* Http::cloneNew() const { return new Http(); }
Http* Http::clone() const { return new Http(*this); }

