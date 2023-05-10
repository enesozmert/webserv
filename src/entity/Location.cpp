#include "../inc/entity/Location.hpp"

Location::Location(/* args */)
{
    this->keywordFill();
}

Location::~Location()
{
}

Location::Location(const Location &location)
{
    *this = location;
}

Location	&Location::operator=(const Location &location)
{
    if (this == &location)
        return (*this);
    this->_path = location._path;
    this->_root = location._root;
    this->_autoindex = location._autoindex;
    this->_index = location._index;
    this->_allowMethods = location._allowMethods;
    this->_clientBodyBufferSize = location._clientBodyBufferSize;
    return (*this);
}

std::string Location::getRoot()
{
    return (this->_root);
}
std::string Location::getAutoindex()
{
    return (this->_autoindex);
}
std::string Location::getIndex()
{
    return (this->_index);
}
std::vector<std::string> Location::getAllowMethods()
{
    std::vector<std::string> allowMethods;
    std::string tempAllowMethods;
    std::stringstream sp(this->_allowMethods);

    while (sp >> std::ws >> tempAllowMethods)
    {
        allowMethods.push_back(tempAllowMethods);
    }
    return (allowMethods);
}
ErrorPage Location::getErrorPage()
{
    return (this->_errorPage);
}
RedirectionUri Location::getRedirectionUri()
{
    return (this->_redirectionUri);
}

DataBase<Variable<std::string> > Location::getKeywordDataBase()
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
void Location::setAutoindex(std::string autoIndex)
{
    this->_autoindex = autoIndex;
}
void Location::setAllowMethods(std::string allowMethods)
{
    (void)allowMethods;
}
void Location::setErrorPage(ErrorPage errorPage)
{
    this->_errorPage = errorPage;
}
void Location::setRedirectionUri(RedirectionUri redirectionUri)
{
    this->_redirectionUri = redirectionUri;
}

void Location::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void Location::keywordFill()
{
    _keywordDatabase.insertData(Variable<std::string>("root",&this->_root));
    _keywordDatabase.insertData(Variable<std::string>("index", &this->_index));
    _keywordDatabase.insertData(Variable<std::string>("auto_index", &this->_autoindex));
    _keywordDatabase.insertData(Variable<std::string>("location", &this->_path));
    _keywordDatabase.insertData(Variable<std::string>("allow_methods", &this->_allowMethods));
}

void Location::setPath(std::string path)
{
    this->_path = path;
}

std::string Location::getName() const
{
    return (this->name);
}

Location* Location::cloneNew() const { return new Location(); }
Location* Location::clone() const { return new Location(*this); }
