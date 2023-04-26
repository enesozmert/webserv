#include "../inc/entity/Http.hpp"

Http::Http(/* args */)
{
}


Http::~Http()
{
}

std::string Http::getClientMaxBodySize()
{
    return (this->_clientMaxBodySize);
}
std::vector<Server> Http::getServer()
{
    return (this->_servers);
}
ErrorPage Http::getErrorPage()
{
    return (this->_errorPage);
}

void Http::setClientMaxBodySize(std::string clientMaxBodySize)
{
    this->_clientMaxBodySize = clientMaxBodySize;
}
void Http::setServer(Server server)
{
    this->_servers.push_back(server);
}
void Http::setErrorPage(ErrorPage errorPage)
{
    this->_errorPage = errorPage;
}