#include "../inc/entity/Response.hpp"

Response::Response() {}

Response::~Response() {}

std::string		Response::getStatusMessage()
{
	if (_errors.find(this->statusCode) != _errors.end())
		return _errors[this->statusCode];
	return ("Unknown Code");
}

int	Response::getStatusCode()
{
    return (this->statusCode);
}

std::string	Response::getPath()
{
    return (this->_path);
}

std::string	Response::getCgiPass()
{
    return (this->_cgiPass);
}

std::string	Response::getBody()
{
    return (this->_body);
}

std::string	Response::getMethod()
{
    return (this->_method);
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

void Response::setStaticErrorPage()
{
	this->staticErrorPage.clear();
	this->staticErrorPage[404] = "<!DOCTYPE html>\n<html><title>404</title><body>There was an error finding your error page</body></html>\n";
	this->staticErrorPage[500] = "<!DOCTYPE html>\n<html><title>500</title><body>Server couldn't handle your request. Still, you won't kill it so easily !</body></html>\n";
	this->staticErrorPage[403] = "<!DOCTYPE html>\n<html><title>403</title><body>This request cannot be authorized (invalid permissions or credentials)</body></html>\n";
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


void	Response::setIndexs(std::vector<std::string> _locationIndex, std::vector<std::string> _serverIndex)
{
	//_locationIndex ve _serverIndex ayrı ayrı vector olarak al
	for (std::vector<std::string>::iterator it = _locationIndex.begin(); it != _locationIndex.end(); it++)
		this->_indexs.push_back(*it);
	for (std::vector<std::string>::iterator itt = _serverIndex.begin(); itt != _serverIndex.end(); itt++)
		this->_indexs.push_back(*itt);
}

std::string Response::selectIndex()
{
	for(std::vector<std::string>::iterator it = this->_indexs.begin(); it != this->_indexs.end(); it++){
		if (!pathIsFile(*it))
			return(*it);
	}
	std::cerr << RED << "No index found" << RESET << std::endl;
    return NULL;
}

int Response::setPaths(ServerScope *server, LocationScope *location)
{
	this->_contentLocation = selectIndex();
	this->_path = location->getRoot() + this->_contentLocation;//this->_path = "./tests/test1/index.html";
	this->_cgi_pass = location->getCgiPass();

	std::cout << YELLOW << "_cgi_pass : " << this->_cgi_pass << RESET << std::endl;
	std::cout << YELLOW << "_contentLocation : " << this->_contentLocation << RESET << std::endl;
	std::cout << YELLOW << "_path : " << this->_path << RESET << std::endl;
}

void Response::setClientBodyBufferSize(std::string bodyBufferSize)
{
	this->_clientBodybufferSize = atoi(bodyBufferSize.c_str());
}

int Response::setResponse(Request *request, ServerScope *server, LocationScope *location)
{
	this->statusCode = request->getReturnCode();//statusCode 200 olarak initledik. İlk 200 olarak atanacak.
	this->_body = request->getBody();
	this->_server = "webserv";
	this->_error_page = location->getErrorPage();
	this->_method = request->getHttpMethodName();
	this->_contentLanguage = request->getAcceptLanguages().front().first;
	setErrors();
	setStaticErrorPage();
	setDate();
	setLastModified();
	setAllowMethods(location->getAllowMethods());
  	setIndexs(location->getIndex(), server->getIndex());//index yoksa hata mı vermeli?
	setPaths(server, location);
	setClientBodyBufferSize(location->getClientBodyBufferSize());
}

void    Response::createResponse(Request *request, ServerScope *server, LocationScope *location)
{
	if (setResponse(request, server, location) == -1)
		std::cerr << RED << "Error setting response" << RESET << std::endl;
	
    if (std::find(_allow_methods.begin(), _allow_methods.end(), this->_method) == _allow_methods.end())
	{
		this->statusCode = 405;
		_response = notAllowed() + "\r\n";
		return ;

	}
	else if (this->_clientBodybufferSize < static_cast<int>(this->_body.size()))
	{
		this->statusCode = 413;
		_response = notAllowed() + "\r\n";
		return ;
	}

    if (this->statusCode == 200 && this->_method == "GET")
        GET_method(request, server);
    else if (this->statusCode == 200 && this->_method == "POST")
        POST_method(request, server);
    else if (this->statusCode == 200 && this->_method == "DELETE")
        DELETE_method();
}

std::string		Response::notAllowed()
{
	std::string	header;

	_response = "";

	setContentType();
	this->_contentLength = std::to_string(this->_response.size());

	if (this->statusCode == 405)
		header = "HTTP/1.1 405 Method Not Allowed\r\n";
	else if (this->statusCode == 413)
		header = "HTTP/1.1 413 Payload Too Large\r\n";
	header += writeHeader();

	return (header);
}

void			Response::GET_method(Request* request, ServerScope* server)
{
	if (this->_cgi_pass != "")
	{
		Cgi	cgi(request, server, *this);
		size_t		i = 0;
		size_t		j = _response.size() - 2;

		_response = cgi.executeCgi(this->_cgi_pass);
		while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
		{
			std::string	str = _response.substr(i, _response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				this->statusCode = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-type: ") == 0)
				this->_contentType = str.substr(14, str.size());
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
		_response = staticErrorPage[500];
	
	_response = getHeader() + "\r\n" + _response;
}

void			Response::DELETE_method()
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

void			Response::POST_method(Request* request, ServerScope* server)
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
		_response = staticErrorPage[500];
	
	_response = getHeader() + "\r\n" + _response;
}

std::string		Response::readHtml()
{
	std::ofstream		file;
	std::stringstream	buffer;

	if (this->statusCode == 403)
		return (staticErrorPage[403]);

	if (pathIsFile(this->_error_page))
	{
		file.open(this->_error_page.c_str(), std::ifstream::in);
		if (file.is_open() == false)
			return (staticErrorPage[404]);

		buffer << file.rdbuf();
		file.close();
		_type = "text/html";

		return (buffer.str());
	}
	else
		return (staticErrorPage[404]);
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
			_response = staticErrorPage[403];
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
	if (_server != "")
		header += "Server: " + _server + "\r\n";
	header += "\r\n";
	return (header);
}

std::string		Response::getHeader()
{
	std::string	header;

	setContentType();
	this->_contentLength = std::to_string(this->_response.size());

	header = "HTTP/1.1 " + std::to_string(this->statusCode) + " " + getStatusMessage() + "\r\n";
	header += writeHeader();

	return (header);
}

