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
    this->_serverNames.push_back(serverName);
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

void Server::setKeywordDatabase(DataBase<std::string> keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
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
    return this->_serverNames;
}

std::vector<Location> Server::getLocations()
{
    return (this->_locations);
}

bool Server::getIsServerNameNothing()
{
    return (this->_isServerNameNothing);
}

DataBase<std::string> Server::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}

void Server::keywordFill()
{

    std::vector<std::string>  datas;

    datas.push_back("listen");
    datas.push_back("server_name");
    datas.push_back("root");
    datas.push_back("index");
    datas.push_back("cgi_pass");

    this->setKeywordDatabase(datas);
}

std::string Server::getName() const
{
    return (this->name);
}

Server* Server::clone() const { return new Server(*this); }
