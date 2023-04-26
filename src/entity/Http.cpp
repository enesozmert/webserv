#include "../inc/entity/Http.hpp"

Http::Http(/* args */)
{
}

Http::Http(char *file)
{
    parseConfig(file);
}


Http::~Http()
{
}



std::string Http::getClientMaxBodySize()
{
    return (this->clientMaxBodySize);
}
Server Http::getServer()
{
    return (this->server);
}
ErrorPage Http::getErrorPage()
{
    return (this->errorPage);
}

void Http::setClientMaxBodySize(std::string clientMaxBodySize)
{
    this->clientMaxBodySize = clientMaxBodySize;
}
void Http::setServer(Server server)
{
    this->server = server;
}
void Http::setErrorPage(ErrorPage errorPage)
{
    this->errorPage = errorPage;
}

void Http::parseConfig(char *file)
{
    std::ifstream ifs(file);
    if (ifs.is_open())
    {

    }
}