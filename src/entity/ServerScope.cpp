#include "../inc/entity/ServerScope.hpp"

ServerScope::ServerScope()
{
    this->keywordFill();
}

ServerScope::~ServerScope() {}

ServerScope::ServerScope(const ServerScope &server) : CgiScope(server)
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
    this->_listen = server._listen;
    this->listen = server.listen;
    this->_locations = server._locations;
    this->_ip = server._ip;
    this->_isServerNameNothing = server._isServerNameNothing;
    this->_errorPage = server._errorPage;
    this->_keywordDatabase = server._keywordDatabase;
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

 void ServerScope::setIp(std::string ip)
 {
    this->_ip = ip;
 }

void ServerScope::setServerName(std::string serverName)
{
    (void)serverName;
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

void ServerScope::setListen()
{
    this->listen.port = atoi(this->getPort().c_str());
    this->listen.host = strToIp(this->getIp());
}

t_listen    ServerScope::getListen()
{
    ServerScope::setListen();
    return this->listen;
}

std::string ServerScope::getPort()
{
    std::string tempHost;
    std::istringstream sp(this->_listen);

    if (this->_listen.find(":") == std::string::npos)
        return tempHost;
    std::getline(sp >> std::ws, tempHost, ':');
    std::getline(sp >> std::ws, tempHost);
    return tempHost;
}
std::string ServerScope::getIp()
{
    std::string tempPort;
    std::istringstream sp(this->_listen);

    std::getline(sp, tempPort, ':');
    return tempPort;
}
std::string ServerScope::getHost()
{
    return (this->_listen);
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
std::vector<std::string> ServerScope::getIndex()
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
    _keywordDatabase.insertData(Variable<std::string>("listen", &this->_listen));
    _keywordDatabase.insertData(Variable<std::string>("cgi_pass", &this->_pass));
    _keywordDatabase.insertData(Variable<std::string>("cgi_param", &this->_param));
}

ServerScope *ServerScope::cloneNew() const { return new ServerScope(); }
ServerScope *ServerScope::clone() const { return new ServerScope(*this); }
std::string ServerScope::getName() const { return (this->name); }

