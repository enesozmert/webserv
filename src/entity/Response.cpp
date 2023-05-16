#include "Response.hpp"
Response::Response(){}

Response::~Response(){}

void    Response::createResponse(Request *request, ServerScope *server, LocationScope *location)
{
    //_errorMap = requestConf.getErrorPage();
    if (location->getAutoindex() == "on")
        this->_isAutoIndex = true;
    else if (location->getAutoindex() == "off")
        this->_isAutoIndex = false;

	this->statusCode = request->getReturnCode();

	//_path = requestConf.getPath();

    if (std::find(location->getAllowMethods().begin(), location->getAllowMethods().end(), request->getHttpMethodName()) == location->getAllowMethods().end())
		this->statusCode = 405;
	else if (location->getClientBodyBufferSize() < request->getBody().size())
		this->statusCode = 413;

	if (this->statusCode == 405 || this->statusCode == 413)
	{
		ResponseHeader	head;

		_response = head.notAllowed(requestConf.getAllowedMethods(), requestConf.getContentLocation(), _code, requestConf.getLang()) + "\r\n";
		return ;
	}


	(this->*Response::_method[request.getMethod()])(request, requestConf);

}

std::string		Response::getResponse()
{
    return (this->_response);
}