#include "../inc/server/Client.hpp"

Client::Client() {};

Client::Client(Server *sv)
{
	this->_response = "";
    this->_isFav = 0;
	this->sv = sv;
	this->multi = 0;
	this->status = 0;
	this->postLen = 0;
	this->postVal = "";
	this->_parser = NULL;
	this->_request = NULL;
	this->_http = sv->getHttp();
	this->response = NULL;
	this->locationIndex = 0;
	this->_response = "";
	this->_host = sv->getHost();
	this->_port = sv->getPort();
}

Client::Client(const Client &client)
{
	*this = client;
}

Client& Client::operator=(const Client &client)
{
	if (this == &client)
        return (*this);
	this->_response = client._response;
	this->sv = client.sv;
	this->multi = client.multi;
	this->status = client.status;
	this->postLen = client.postLen;
	this->postVal = client.postVal;
	this->_parser = client._parser;
	this->_request = client._request;
	this->_http = client._http;
	this->response = client.response;
	this->locationIndex = client.locationIndex;
	this->_response = client._response;
	this->_host = client._host;
	this->_port = client._port;
    this->_isFav = client._isFav;
	return (*this);
}

Client::~Client() {}

int Client::getIsFav() const
{
    return this->_isFav;

}

int	Client::getStatus() const
{
	return this->status;
}

int	Client::getMulti() const
{
	return this->multi;
}

std::string		Client::getPostVal() const
{
	return this->postVal;
}

size_t			Client::getPostLen() const
{
	return this->postLen;
}

void   Client::setRequest(Request *request)
{
    this->_request = request;
}

void   Client::setParserRequest(std::string buffer)
{
    std::cout << RED << "setParseRequest'e girdi " << RESET << std::endl;
    this->_parser = new ParserRequest();
    this->_parser->parse(buffer);
    this->status = this->_parser->getStatus();
    this->multi = this->_parser->getMulti();
    setRequest(this->_parser->getRequest());
    if (this->_request->getPath() == "/favicon.ico" || _request->getPath() == "favicon.ico")
    {
        std::cout << "favicon geldi" << std::endl;
        this->_isFav = 1;
    }
	this->postLen = this->_request->getContentLength();
    delete _parser;
}

std::string    Client::process()
{
    std::cout << RED << "Processing" << RESET << std::endl;
    ServerScope     *matchedServer;
    LocationScope   *matchedLocation;
    this->response = new Response();

    matchedServer = this->getServerForRequest();
    this->getLocationForRequest(matchedServer, _request->getPath());

    if (matchedServer == NULL || this->locationIndex == -1)
    {
        std::cerr << "No matched server/location found" << std::endl;
        return NULL;
    }
    matchedLocation = matchedServer->getLocations().at(this->locationIndex);
    this->_response = this->response->createResponse(_request, matchedServer, matchedLocation, postVal);
    if (this->_response == "")
    {
        delete this->response;
        return NULL;
    }
    delete this->response;
    return _response;
}

ServerScope*        Client::getServerForRequest()
{
    std::vector<ServerScope *>  serverScope;

    serverScope = this->_http->getServers();
    for (std::vector<ServerScope *>::const_iterator it = serverScope.begin() ; it != serverScope.end(); it++)
    {
        if (this->_host == (*it)->getListen().host && this->_port == (*it)->getListen().port)
            return(*it);
    }
    std::cerr << RED << "There is no possible server" << RESET << std::endl;
    return NULL;
}


//benim yazdığım daha basic olan
void  Client::getLocationForRequest(ServerScope *matchedServerScope, const std::string& path) 
{
    this->locationIndex = 0;

    this->locationIndex = getMatchLocationPathIndex(matchedServerScope, path);
    if(this->locationIndex == -1)
        std::cerr << RED << "There is no possible location" << RESET << std::endl;
}
