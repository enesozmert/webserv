#include "../inc/entity/Location.hpp"

Location::Location(/* args */)
{
    this->keywordFill();

}
Location::~Location()
{
}

std::string Location::getRoot()
{
    return (this->_root);
}
bool Location::getAutoindex()
{
    return (this->_autoindex);
}
std::string Location::getIndex()
{
    return (this->_index);
}
ErrorPage Location::getErrorPage()
{
    return (this->_errorPage);
}
RedirectionUri Location::getRedirectionUri()
{
    return (this->_redirectionUri);
}

DataBase<std::string> Location::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}

std::string Location::getPath()
{
    return (this->_path);
}


void Location::setRoot(std::string root)
{
    this->_root = root;
}
void Location::setIndex(std::string index)
{
    this->_index = index;
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

void Location::setKeywordDatabase(DataBase<std::string> keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void Location::keywordFill()
{
    std::vector<std::string>  datas;

    datas.push_back("allow");
    datas.push_back("root");
    datas.push_back("index");

    this->setKeywordDatabase(datas);
}

void Location::setPath(std::string path)
{
    this->_path = path;
}

std::string Location::getName() const
{
    return (this->name);
}