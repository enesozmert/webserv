#include "../inc/entity/ServerScope.hpp"

ServerScope::ServerScope(/* args */)
{
    this->keywordFill();
}

ServerScope::~ServerScope()
{
}

ServerScope::ServerScope(const ServerScope &server)
{
    *this = server;
}

ServerScope	&ServerScope::operator=(const ServerScope &server)
{
    if (this == &server)
        return (*this);
    this->_port = server._port;
    this->_host = server._host;
    this->_root = server._root;
    this->_index = server._index;
    this->_serverNames = server._serverNames;
    this->_cgi_pass = server._cgi_pass;
    this->_listen = server._listen;
    return (*this);
}

void ServerScope::setHost(std::string host)
{
    this->_host = host;
}
void ServerScope::setPort(std::string port)
{
    this->_port = port;
}

void ServerScope::setServerName(std::string serverName)
{
    (void)serverName;
    // this->_serverNames.push_back(serverName);
}

void ServerScope::setLocation(LocationScope *location)
{
    this->_locations.push_back(location);
}

void ServerScope::setErrorPage(ErrorPage errorPage)
{
    this->_errorPage = errorPage;
}

void ServerScope::setIsServerNameNothing(bool isServerNameNothing)
{
    this->_isServerNameNothing = isServerNameNothing;
}

void ServerScope::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void ServerScope::setRoot(std::string root)
{
    this->_root = root;
}
void ServerScope::setIndex(std::string index)
{
    this->_index = index;
}
void ServerScope::setCgi_pass(std::string cgi_pass)
{
    this->_cgi_pass = cgi_pass;
}

void ServerScope::setListen(std::string port, std::string host)
{
    this->listen.port = atoi(this->getPort().c_str());
    this->listen.host = strToIp(this->getHost());
}

//default location initialization yap
//en genel ayarlara sahip olanı seçebilirsin. Örneğin, tüm istekleri karşılamak için "/", "*/", ya da "/default" gibi bir path'e sahip olanı
/*void ServerScope::setDefaultLocation()
{
}*/

t_listen    ServerScope::getListen() const
{
    return this->listen;
}

std::string ServerScope::getPort()
{
    std::string tempPort;
    std::istringstream sp(this->_listen);

    std::getline(sp, tempPort, ':');
    return tempPort;
}
std::string ServerScope::getHost()
{
    std::string tempHost;
    std::istringstream sp(this->_listen);

    if (this->_listen.find(":") == std::string::npos)
        return tempHost;
    std::getline(sp >> std::ws, tempHost, ':');
    std::getline(sp >> std::ws, tempHost);
    return tempHost;
}
ErrorPage ServerScope::getErrorPage()
{
    return this->_errorPage;
}
std::vector<std::string> ServerScope::getServerName()
{
    std::vector<std::string> serverNames;
    std::string tempServerName;
    std::stringstream sp(this->_serverNames);

    while (sp >> std::ws >> tempServerName)
    {
        serverNames.push_back(tempServerName);
    }
    return (serverNames);
}

std::vector<LocationScope *> ServerScope::getLocations()
{
    return (this->_locations);
}

bool ServerScope::getIsServerNameNothing()
{
    return (this->_isServerNameNothing);
}

std::string ServerScope::getRoot()
{
    return (this->_root);
}
std::string ServerScope::getIndex()
{
    return (this->_index);
}
std::string ServerScope::getCgi_pass()
{
    return (this->_cgi_pass);
}

DataBase<Variable<std::string> > ServerScope::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}

void ServerScope::keywordFill()
{
    _keywordDatabase.insertData(Variable<std::string>("port", &this->_port));
    _keywordDatabase.insertData(Variable<std::string>("host", &this->_host));
    _keywordDatabase.insertData(Variable<std::string>("server_name", &this->_serverNames));
    _keywordDatabase.insertData(Variable<std::string>("root", &this->_root));
    _keywordDatabase.insertData(Variable<std::string>("index", &this->_index));
    _keywordDatabase.insertData(Variable<std::string>("cgi_pass", &this->_cgi_pass));
    _keywordDatabase.insertData(Variable<std::string>("listen", &this->_listen));
}

std::string ServerScope::getName() const
{
    return (this->name);
}

ServerScope *ServerScope::cloneNew() const { return new ServerScope(); }
ServerScope *ServerScope::clone() const { return new ServerScope(*this); }

//fatma ekledi
LocationScope* ServerScope::getDefaultLocation() const {
    // Eğer default location tanımlanmamışsa, null döndürür
    if (defaultLocation == nullptr) {
        return nullptr;
    }
    // Eğer default location tanımlanmışsa, ilgili location döndürür
    else {
        return defaultLocation;
    }
}