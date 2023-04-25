#include "../inc/entity/Location.hpp"

Location::Location(/* args */)
{
}
Location::~Location()
{
}

std::string Location::getRoot()
{
    return (this->root);
}
bool Location::getAutoindex()
{
    return (this->_autoindex);
}
std::string Location::getIndex()
{
    return (this->index);
}
ErrorPage Location::getErrorPage()
{
    return (this->_errorPage);
}
RedirectionUri Location::getRedirectionUri()
{
    return (this->_redirectionUri);
}

void Location::setRoot(std::string root)
{
    this->root = root;
}
void Location::setIndex(std::string index)
{
    this->index = index;
}
void Location::setAutoindex(bool autoIndex)
{
    this->_autoindex = autoIndex;
}
void Location::setErrorPage(ErrorPage errorPage)
{
    this->_errorPage = errorPage;
}
void Location::setRedirectionUri(RedirectionUri redirectionUri)
{
    this->_redirectionUri = redirectionUri;
}