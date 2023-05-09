#include "../inc/entity/Server.hpp"


Server::Server(/* args */)
{ 
    this->keywordFill();
}

Server::~Server(){

}

void Server::setHost(std::string host){
    this->_host = host;
}
void Server::setPort(std::string port){
    this->_port = port;
}

void Server::setServerName(std::string serverName){
    (void)serverName;
    // this->_serverNames.push_back(serverName);
}

void Server::setLocation(Location location)
{
    this->_locations.push_back(location);
}

void Server::setErrorPage(ErrorPage errorPage){
    this->_errorPage = errorPage;
}

void Server::setIsServerNameNothing(bool isServerNameNothing)
{
    this->_isServerNameNothing = isServerNameNothing;
}

void Server::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void Server::setRoot(std::string root)
{
    this->_root = root;
}
void Server::setIndex(std::string index)
{
    this->_index = index;
}
void Server::setCgi_pass(std::string cgi_pass)
{
    this->_cgi_pass = cgi_pass;
}

std::string Server::getPort(){
    return this->_port;
}
std::string Server::getHost(){
    return this->_host;
}
ErrorPage Server::getErrorPage(){
    return this->_errorPage;
}
std::vector<std::string> Server::getServerName(){
    std::vector<std::string> serverNames;
    std::string tempServerName;
    std::stringstream sp(this->_serverNames);

    while (sp << tempServerName)
    {
        serverNames.push_back(tempServerName);
    }
    return (serverNames);
}

std::vector<Location> Server::getLocations()
{
    return (this->_locations);
}

bool Server::getIsServerNameNothing()
{
    return (this->_isServerNameNothing);
}

std::string Server::getRoot()
{
    return (this->_root);
}
std::string Server::getIndex()
{
    return (this->_index);
}
std::string Server::getCgi_pass()
{
    return (this->_cgi_pass);
}

DataBase<Variable<std::string> > Server::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}

void Server::keywordFill()
{
    _keywordDatabase.insertData(Variable<std::string>("port", &this->_port));
    _keywordDatabase.insertData(Variable<std::string>("host", &this->_host));
    _keywordDatabase.insertData(Variable<std::string>("server_name", &this->_serverNames));
    _keywordDatabase.insertData(Variable<std::string>("root", &this->_root));
    _keywordDatabase.insertData(Variable<std::string>("index", &this->_index));
    _keywordDatabase.insertData(Variable<std::string>("cgi_pass", &this->_cgi_pass));
}

std::string Server::getName() const
{
    return (this->name);
}

Server* Server::clone() const { return new Server(); }
