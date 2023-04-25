#include "../inc/entity/Server.hpp"


Server::Server(/* args */)
{ 
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
    this->_location = location;
}

void Server::setErrorPage(ErrorPage errorPage){
    this->_errorPage = errorPage;
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

Location Server::getLocation()
{
    return (this->_location);
}