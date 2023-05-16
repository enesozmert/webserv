#include "Response.hpp"

Response::Response(){}

Response::~Response(){}

void			Response::setAllow(std::vector<std::string> methods)
{
	std::vector<std::string>::iterator it = methods.begin();

	while (it != methods.end())
	{
		this->_allow += *(it++);

		if (it != methods.end())
			this->_allow += ", ";
	}
}

void			Response::setAllow(const std::string& allow)
{
	this->_allow = allow;
}

void			Response::setContentLanguage(const std::string& lang)
{
	_contentLanguage = lang;
}

void			Response::setContentLength(size_t size)
{
	_contentLength = to_string(size);
}

void			Response::setContentLocation(const std::string& path, int code)
{
	(void)code;
	// if (code != 404)
		_contentLocation = path;
}

void			Response::setContentType(std::string type, std::string path)
{
	if (type != "")
	{
		_contentType = type;
		return ;
	}
	type = path.substr(path.rfind(".") + 1, path.size() - path.rfind("."));
	if (type == "html")
		_contentType = "text/html";
	else if (type == "css")
		_contentType = "text/css";
	else if (type == "js")
		_contentType = "text/javascript";
	else if (type == "jpeg" || type == "jpg")
		_contentType = "image/jpeg";
	else if (type == "png")
		_contentType = "image/png";
	else if (type == "bmp")
		_contentType = "image/bmp";
	else
		_contentType = "text/plain";
}

void			Response::setDate(void)
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	_date = std::string(buffer);
}

void			Response::setLastModified(const std::string& path)
{
	char			buffer[100];
	struct stat		stats;
	struct tm		*tm;

	if (stat(path.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		_lastModified = std::string(buffer);
	}
}

void			Response::setLocation(int code, const std::string& redirect)
{
	if (code == 201 || code / 100 == 3)
	{
		_location = redirect;
	}
}

void			Response::setRetryAfter(int code, int sec)
{
	if (code == 503 || code == 429 || code == 301)
	{
		_retryAfter = to_string(sec);
	}
}

void			Response::setServer(void)
{
	_server = "Weebserv/1.0.0 (Unix)";
}

void			Response::setTransferEncoding(void)
{
	_transferEncoding = "identity";
}

void			Response::setWwwAuthenticate(int code)
{
	if (code == 401)
	{
		_wwwAuthenticate = "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
	}
}


void    Response::createResponse(Request *request, ServerScope *server, LocationScope *location)
{
    //_errorMap = request->getErrorPage();
    this->_cgi_param = request->getEnvForCgi();
	this->statusCode = request->getReturnCode();
    //addIndex(request);
 /*    std::vector<std::string> conf_index = server->getIndex();
	for (std::vector<std::string>::const_iterator it = conf_index.begin(); it != conf_index.end(); it++)
	{
		std::vector<std::string>::const_iterator it2 = _index.begin();
		for (it2 = _index.begin(); it2 != _index.end(); it2++)
		{
				if (*it == *it2)
					break;
		}
		if (it2 == _index.end())
			_index.push_back(*it);
	} */

    this->_contentLocation = removeAdjacentSlashes(request->getPath());
	this->_path = removeAdjacentSlashes(server->getRoot() + location->getPath());

    if (location->getAutoindex() == "on")
        this->_isAutoIndex = true;
    else if (location->getAutoindex() == "off")
        this->_isAutoIndex = false;


    if (std::find(location->getAllowMethods().begin(), location->getAllowMethods().end(), request->getHttpMethodName()) == location->getAllowMethods().end())
		this->statusCode = 405;
	else if (atoi(location->getClientBodyBufferSize().c_str()) < request->getBody().size())
		this->statusCode = 413;

    if (this->statusCode == 405 || this->statusCode == 413)
	{
		_response = notAllowed(location->getAllowMethods(), location->getPath(), this->statusCode, request->getAcceptLanguages().front()) + "\r\n";
		return ;
	}

    if (this->statusCode == 200)
    {
        if (request->getHttpMethodName() == "GET")
            GETmethod(request, server, location);
        else if (request->getHttpMethodName() == "POST")
            POSTmethod(request, server, location);
        else if (request->getHttpMethodName() == "POST")
            DELETEmethod(request, server, location);
        else
            std::cerr << "buraya ne eklememiz lazım bilemedim" << std::endl;   
    }
}

std::string		Response::getResponse()
{
    return (this->_response);
}

int	Response::getStatusCode()
{
    return (this->statusCode);
}

void			Response::setValues(size_t size, const std::string& path, int code, std::string type, const std::string& contentLocation, const std::string& lang)
{
	setAllow();
	setContentLanguage(lang);
	setContentLength(size);
	setContentLocation(contentLocation, code);
	setContentType(type, path);
	setDate();
	setLastModified(path);
	setLocation(code, path);
	setRetryAfter(code, 3);
	setServer();
	setTransferEncoding();
	setWwwAuthenticate(code);
}

void			Response::resetValues(void)
{
	_allow = "";
	_contentLanguage = "";
	_contentLength = "";
	_contentLocation = "";
	_contentType = "";
	_date = "";
	_lastModified = "";
	_location = "";
	_retryAfter = "";
	_server = "";
	_transferEncoding = "";
	_wwwAuthenticate = "";
	//initErrorMap();
}

std::string		Response::writeHeader(void)
{
	std::string	header = "";

	if (_allow != "")
		header += "Allow: " + _allow + "\r\n";
	if (_contentLanguage != "")
		header += "Content-Language: " + _contentLanguage + "\r\n";
	if (_contentLength != "")
		header += "Content-Length: " + _contentLength + "\r\n";
	if (_contentLocation != "")
		header += "Content-Location: " + _contentLocation + "\r\n";
	if (_contentType != "")
		header += "Content-Type: " + _contentType + "\r\n";
	if (_date != "")
		header += "Date: " + _date + "\r\n";
	if (_lastModified != "")
		header += "Last-Modified: " + _lastModified + "\r\n";
	if (_location != "")
		header += "Location: " + _location + "\r\n";
	if (_retryAfter != "")
		header += "Retry-After: " + _retryAfter + "\r\n";
	if (_server != "")
		header += "Server: " + _server + "\r\n";
	if (_transferEncoding != "")
		header += "Transfer-Encoding: " + _transferEncoding + "\r\n";
	if (_wwwAuthenticate != "")
		header += "WWW-Authenticate: " + _wwwAuthenticate + "\r\n";
	// header += "\r\n";

	return (header);
}


std::string		Response::notAllowed(std::vector<std::string> methods, const std::string& path, int code, const std::string& lang)
{
	std::string	header;

	resetValues();
	setValues(0, path, code, "", path, lang);
	setAllow(methods);

	if (code == 405)
		header = "HTTP/1.1 405 Method Not Allowed\r\n";
	else if (code == 413)
		header = "HTTP/1.1 413 Payload Too Large\r\n";
	header += writeHeader();

	return (header);
}

void			Response::GETmethod(Request* request, ServerScope* scope, LocationScope* location)
{
	if (request->getCgiPass() != "")
	{
		CgiHandler	cgi(request, requestConf);
		size_t		i = 0;
		size_t		j = _response.size() - 2;

		_response = cgi.executeCgi(requestConf.getCgiPass());

		while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
		{
			std::string	str = _response.substr(i, _response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				_code = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-type: ") == 0)
				_type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (_response.find("\r\n", j) == j)
			j -= 2;

		_response = _response.substr(i, j - i);
	}
	else if  (_code == 200)
		_code = readContent();
	else
		_response = this->readHtml(_errorMap[_code]);
	if (_code == 500)
		_response = this->readHtml(_errorMap[_code]);

	_response = head.getHeader(_response.size(), _path, _code, _type, requestConf.getContentLocation(), requestConf.getLang()) + "\r\n" + _response;
}

void			Response::DELETEmethod(Request* request, ServerScope* scope, LocationScope* location)
{
	ResponseHeader	head;
	(void)request;

	_response = "";
	if (pathIsFile(_path))
	{
		if (remove(_path.c_str()) == 0)
			_code = 204;
		else
			_code = 403;
	}
	else
		_code = 404;
	if (_code == 403 || _code == 404)
		_response = this->readHtml(_errorMap[_code]);
	_response = head.getHeader(_response.size(), _path, _code, _type, requestConf.getContentLocation(), requestConf.getLang()) + "\r\n" + _response;
}

void			Response::POSTmethod(Request* request, ServerScope* scope, LocationScope* location)
{
	ResponseHeader	head;

	if (requestConf.getCgiPass() != "")
	{
		CgiHandler	cgi(request, requestConf);
		size_t		i = 0;
		size_t		j = _response.size() - 2;

		_response = cgi.executeCgi(requestConf.getCgiPass());

		while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
		{
			std::string	str = _response.substr(i, _response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				_code = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-Type: ") == 0)
				_type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (_response.find("\r\n", j) == j)
			j -= 2;

		_response = _response.substr(i, j - i);
	}
	else
	{
		_code = 204;
		_response = "";
	}
	if (_code == 500)
		_response = this->readHtml(_errorMap[_code]);
	_response = head.getHeader(_response.size(), _path, _code, _type, requestConf.getContentLocation(), requestConf.getLang()) + "\r\n" + _response;
}
