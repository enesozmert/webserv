#include "../inc/entity/Http.hpp"

Http::Http(/* args */)
{
}

Http::~Http()
{
}

std::string getClientMaxBodySize()
{
    return (this->clientMaxBodySize);
}
Server getServer()
{
    return (this->server);
}
ErrorPage getErrorPage()
{
    return (this->errorPage);
}

void setClientMaxBodySize(std::string clientMaxBodySize)
{
    this->clientMaxBodySize = clientMaxBodySize;
}
void setServer(Server server)
{
    this->server = server;
}
void setErrorPage(ErrorPage errorPage)
{
    this->errorPage = errorPage;
}
