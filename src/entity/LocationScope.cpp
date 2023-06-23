#include "../inc/entity/LocationScope.hpp"

LocationScope::LocationScope()
{
    this->keywordFill();
}

LocationScope::~LocationScope() {}

LocationScope::LocationScope(const LocationScope &location) : CgiScope(location),
    ErrorPage(location), RedirectionUri(location)
{
    *this = location;
}

LocationScope	&LocationScope::operator=(const LocationScope &location)
{
    if (this == &location)
        return (*this);
    this->_path = location._path;
    this->_root = location._root;
    this->_autoindex = location._autoindex;
    this->_index = location._index;
    this->_allowMethods = location._allowMethods;
    this->_clientBodyBufferSize = location._clientBodyBufferSize;
    this->_keywordDatabase = location._keywordDatabase;
    return (*this);
}

std::string LocationScope::getRoot()
{
    return (this->_root);
}
std::string LocationScope::getAutoindex()
{
    return (this->_autoindex);
}
std::vector<std::string> LocationScope::getIndex()
{
    std::vector<std::string> indexs;
    std::string tempIndexs;
    std::stringstream sp(this->_index);

    while (sp >> std::ws >> tempIndexs)
    {
        indexs.push_back(tempIndexs);
    }
    return (indexs);
}
std::vector<std::string> LocationScope::getAllowMethods()
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

DataBase<Variable<std::string> > LocationScope::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}

std::string LocationScope::getPath()
{
    return (this->_path);
}

std::string LocationScope::getClientBodyBufferSize()
{
    return (this->_clientBodyBufferSize);
}
void LocationScope::setRoot(std::string root)
{
    this->_root = root;
}
void LocationScope::setIndex(std::string index)
{
    this->_index = index;
}
void LocationScope::setAutoindex(std::string autoIndex)
{
    this->_autoindex = autoIndex;
}
void LocationScope::setAllowMethods(std::string allowMethods)
{
    (void)allowMethods;
}

void LocationScope::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void LocationScope::setPath(std::string path)
{
    this->_path = path;
}

void LocationScope::keywordFill()
{
    _keywordDatabase.insertData(Variable<std::string>("root",&this->_root));
    _keywordDatabase.insertData(Variable<std::string>("index", &this->_index));
    _keywordDatabase.insertData(Variable<std::string>("auto_index", &this->_autoindex));
    _keywordDatabase.insertData(Variable<std::string>("location", &this->_path));
    _keywordDatabase.insertData(Variable<std::string>("allow_methods", &this->_allowMethods));
    _keywordDatabase.insertData(Variable<std::string>("cgi_pass", &this->_pass));
    _keywordDatabase.insertData(Variable<std::string>("cgi_param", &this->_param));
    _keywordDatabase.insertData(Variable<std::string>("error_page", &this->_errorPage));
    _keywordDatabase.insertData(Variable<std::string>("return", &this->_redirectionUri));
    _keywordDatabase.insertData(Variable<std::string>("request_max_body_size", &this->_clientBodyBufferSize));
}

std::string LocationScope::getName() const { return (this->name); }
LocationScope* LocationScope::cloneNew() const { return new LocationScope(); }
LocationScope* LocationScope::clone() const { return new LocationScope(*this); }