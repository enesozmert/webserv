#include "../inc/entity/Response.hpp"

Response::Response(){
	resetValues();
}

Response::~Response(){}

std::string		Response::getResponse()
{
    return (this->_response);
}

int	Response::getStatusCode()
{
    return (this->statusCode);
}

std::map<int, std::string>  Response::getErrorMap()
{
    return (this->_errorMap);
}

void    Response::setErrors()
{
	this->_errors.clear();
	this->_errors[100] = "Continue";
	this->_errors[200] = "OK";
	this->_errors[201] = "Created";
	this->_errors[204] = "No Content";
	this->_errors[400] = "Bad Request";
	this->_errors[403] = "Forbidden";
	this->_errors[404] = "Not Found";
	this->_errors[405] = "Method Not Allowed";
	this->_errors[413] = "Payload Too Large";
	this->_errors[500] = "Internal Server Error";
}

void    Response::setErrorMap()
{
	this->_errorMap.clear();
    this->_errorMap[400] = "configs/default_error_pages/400.html";
    this->_errorMap[403] = "configs/default_error_pages/403.html";
    this->_errorMap[404] = "configs/default_error_pages/404.html";
    this->_errorMap[405] = "configs/default_error_pages/405.html";
	this->_errorMap[410] = "configs/default_error_pages/410.html";
    this->_errorMap[413] = "configs/default_error_pages/413.html";
    this->_errorMap[500] = "configs/default_error_pages/500.html";
}

void			Response::setAllowMethods(std::vector<std::string> methods)
{
	std::vector<std::string>::iterator it = methods.begin();

	while (it != methods.end())
	{
		this->_allow_methods += *(it++);

		if (it != methods.end())
			this->_allow_methods += ", ";
	}
}

void			Response::setContentLength()
{
	_contentLength = std::to_string(this->_response.size());
}

void			Response::setContentType()
{
	if (this->_type != "")
	{
		_contentType = this->_type;
		return ;
	}
	this->_type = this->_path.substr(this->_path.rfind(".") + 1, this->_path.size() - this->_path.rfind("."));
	if (this->_type == "html")
		_contentType = "text/html";
	else if (this->_type == "css")
		_contentType = "text/css";
	else if (this->_type == "js")
		_contentType = "text/javascript";
	else if (this->_type == "jpeg" || this->_type == "jpg")
		_contentType = "image/jpeg";
	else if (this->_type == "png")
		_contentType = "image/png";
	else if (this->_type == "bmp")
		_contentType = "image/bmp";
	else
		_contentType = "text/plain";
}

void			Response::setDate()
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	_date = std::string(buffer);
}

void			Response::setLastModified()
{
	char			buffer[100];
	struct stat		stats;
	struct tm		*tm;

	if (stat(this->_path.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		_lastModified = std::string(buffer);
	}
}

void			Response::setLocation()
{
	if (this->statusCode == 201 || this->statusCode / 100 == 3)
	{
		_location = this->_path;
	}
}

void			Response::setRetryAfter()
{
	if (this->statusCode == 503 || this->statusCode == 429 || this->statusCode == 301)
	{
		_retryAfter = std::to_string(3);
	}
}

void			Response::setServer(void)
{
	_server = "webserv";
}

void			Response::setTransferEncoding(void)
{
	_transferEncoding = "identity";
}

void			Response::setWwwAuthenticate()
{
	if (this->statusCode == 401)
	{
		_wwwAuthenticate = "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
	}
}

void			Response::setValues()
{
	setErrors();
	setContentLength();
	setContentType();
	setDate();
	setLastModified();
	setLocation();
	setRetryAfter();
	setServer();
	setTransferEncoding();
	setWwwAuthenticate();
}

void	Response::setIndexs(std::vector<std::string> _locationIndex, std::vector<std::string> _serverIndex)
{
	//_locationIndex ve _serverIndex ayrı ayrı vector olarak al
	for (std::vector<std::string>::iterator it = _locationIndex.begin(); it != _locationIndex.end(); it++)
		this->_indexs.push_back(*it);
	for (std::vector<std::string>::iterator itt = _serverIndex.begin(); itt != _serverIndex.end(); itt++)
		this->_indexs.push_back(*itt);
}

void    Response::setParams(std::vector<std::string> _paramKeyword, std::vector<std::string> _paramValue)
{
	int i = 0;
	if (_paramValue.size() == _paramKeyword.size())
	{
		for (std::vector<std::string>::iterator it = _paramValue.begin(); it != _paramValue.end(); it++)
		{
			this->_cgi_params[_paramKeyword.at(i)] = *it;
			i++;
		}
	}
}

std::string Response::selectIndex()
{
	for(std::vector<std::string>::iterator it = this->_indexs.begin(); it != this->_indexs.end(); it++){
		if (!pathIsFile(*it))
			return(*it);
	}
	std::cerr << RED << "No index found" << RESET << std::endl;
    return (0);
}

void    Response::createResponse(Request *request, ServerScope *server, LocationScope *location)
{
	setErrorMap();
	this->_allow = request->getHttpMethodName();
	this->statusCode = request->getReturnCode();//statusCode 200 olarak initledik. İlk 200 olarak atanacak.
	this->_cgi_pass = location->getCgiPass();
  	setIndexs(location->getIndex(), server->getIndex());
	//setParams(location->getParamKeyword(), location->getParamValues());
  	this->_contentLocation = selectIndex();
	this->_path = location->getRoot() + this->_contentLocation;//this->_path = "./tests/test1/index.html";
	this->_contentLanguage = request->getAcceptLanguages().front().first;
	std::cout << YELLOW << "_cgi_pass : " << this->_cgi_pass << RESET << std::endl;
	std::cout << YELLOW << "_contentLocation : " << this->_contentLocation << RESET << std::endl;
	std::cout << YELLOW << "_path : " << this->_path << RESET << std::endl;

    if (std::find(location->getAllowMethods().begin(), location->getAllowMethods().end(), this->_allow) == location->getAllowMethods().end())
		this->statusCode = 405;
	else if (atoi(location->getClientBodyBufferSize().c_str()) < static_cast<int>(request->getBody().size()))
		this->statusCode = 413;

    if (this->statusCode == 405 || this->statusCode == 413)
	{
		_response = notAllowed(location->getAllowMethods()) + "\r\n";
		return ;
	}

    if (this->statusCode == 200)
    {
        if (request->getHttpMethodName() == "GET")
            GETmethod(request, server);
        else if (request->getHttpMethodName() == "POST")
            POSTmethod(request, server);
        else if (request->getHttpMethodName() == "DELETE")
            DELETEmethod();
        else
            std::cerr << "buraya ne eklememiz lazım bilemedim" << std::endl;   
    }
}

std::string		Response::writeHeader(void)
{
	std::string	header = "";

	if (_allow_methods != "")
		header += "Allow: " + _allow_methods + "\r\n";
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
	header += "\r\n";
	return (header);
}


std::string		Response::notAllowed(std::vector<std::string> methods)
{
	std::string	header;

	_response = "";

	resetValues();
	setValues();
	setAllowMethods(methods);

	if (this->statusCode == 405)
		header = "HTTP/1.1 405 Method Not Allowed\r\n";
	else if (this->statusCode == 413)
		header = "HTTP/1.1 413 Payload Too Large\r\n";
	header += writeHeader();

	return (header);
}

void			Response::GETmethod(Request* request, ServerScope* server)
{
	if (this->_cgi_pass != "")
	{
		Cgi	cgi(request, server, this->_path);
		size_t		i = 0;
		size_t		j = _response.size() - 2;

		_response = cgi.executeCgi(this->_cgi_pass);
		while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
		{
			std::string	str = _response.substr(i, _response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				this->statusCode = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-type: ") == 0)
				_type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (_response.find("\r\n", j) == j)
			j -= 2;
		_response = _response.substr(i, j - i);
	}
	else if  (this->statusCode == 200)
		readContent();
	else
		_response = this->readHtml();
	
	if (this->statusCode == 500)
		_response = this->readHtml();

	_response = getHeader() + "\r\n" + _response;
}

void			Response::DELETEmethod()
{
	_response = "";
	if (pathIsFile(_path))
	{
		if (remove(_path.c_str()) == 0)
			this->statusCode = 204;
		else
			this->statusCode = 403;
	}
	else
		this->statusCode = 404;
	if (this->statusCode == 403 || this->statusCode == 404)
		_response = this->readHtml();
	_response = getHeader() + "\r\n" + _response;
}

void			Response::POSTmethod(Request* request, ServerScope* server)
{
	if (this->_cgi_pass != "")
	{
		Cgi	cgi(request, server, this->_path);
		size_t		i = 0;
		size_t		j = _response.size() - 2;

		_response = cgi.executeCgi(this->_cgi_pass);
		while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
		{
			std::string	str = _response.substr(i, _response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				this->statusCode = std::atoi(str.substr(8, 3).c_str());
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
		this->statusCode = 204;
		_response = "";
	}
	if (this->statusCode == 500)
		_response = this->readHtml();
	_response = getHeader() + "\r\n" + _response;
}

std::string		Response::readHtml()
{
	std::ofstream		file;
	std::stringstream	buffer;

	if (pathIsFile(_errorMap[this->statusCode]))
	{
		file.open(_errorMap[this->statusCode].c_str(), std::ifstream::in);
		if (file.is_open() == false)
			return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");

		buffer << file.rdbuf();
		file.close();
		_type = "text/html";

		return (buffer.str());
	}
	else
		return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");
}

void				Response::readContent()
{
	std::ifstream		file;
	std::stringstream	buffer;

	_response = "";

	if (pathIsFile(_path))
	{
		file.open(_path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			this->statusCode = 403;
			_response = this->readHtml();
			return ;
		}
		buffer << file.rdbuf();
		_response = buffer.str();
		file.close();
		return ;
	}
	this->statusCode = 404;
	_response = this->readHtml();
	return ;
}


std::string		Response::getHeader()
{
	std::string	header;

	resetValues();
	setValues();

	header = "HTTP/1.1 " + std::to_string(this->statusCode) + " " + getStatusMessage() + "\r\n";
	header += writeHeader();

	return (header);
}

std::string		Response::getStatusMessage()
{
	if (_errors.find(this->statusCode) != _errors.end())
		return _errors[this->statusCode];
	return ("Unknown Code");
}

void			Response::resetValues()
{
	_allow = "";
	_allow_methods = "";
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