#include "../inc/entity/Response.hpp"

Response::Response(){}

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

void    Response::setErrorMap()
{
	this->_errorMap.clear();
	this->_errorMap[100] = "Continue";
	this->_errorMap[200] = "OK";
	this->_errorMap[201] = "Created";
	this->_errorMap[204] = "No Content";
	this->_errorMap[400] = "Bad Request";
	this->_errorMap[403] = "Forbidden";
	this->_errorMap[404] = "Not Found";
	this->_errorMap[405] = "Method Not Allowed";
	this->_errorMap[413] = "Payload Too Large";
	this->_errorMap[500] = "Internal Server Error";
}

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
	_contentLength = std::to_string(size);
}

void	Response::setContentLocation(const std::string& path)
{
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
		_retryAfter = std::to_string(sec);
	}
}

void			Response::setServer(void)
{
	_server = "webserv";//buraya farklı isim verilebilir mi?
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

void			Response::setValues(size_t size, const std::string& path, int code, std::string type, const std::string& contentLocation, const std::string& lang)
{
	setAllow();
	setErrorMap();
	setContentLanguage(lang);
	setContentLength(size);
	setContentLocation(contentLocation);
	setContentType(type, path);
	setDate();
	setLastModified(path);
	setLocation(code, path);
	setRetryAfter(code, 3);
	setServer();
	setTransferEncoding();
	setWwwAuthenticate(code);
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

void    Response::createResponse(Request *request, ServerScope *server, LocationScope *location)
{
	this->statusCode = request->getReturnCode();//statusCode 200 olarak initledik. İlk 200 olarak atanacak.
	//this->_cgi_pass = location->getPass();
  	setIndexs(location->getIndex(), server->getIndex());
	//setParams(location->getParamKeyword(), location->getParamValues());
  	this->_contentLocation = _indexs.at(0);
	this->_path = location->getRoot() + _indexs.at(0);//this->_path = "./tests/test1/index.html";
	//std::cout << YELLOW << "_cgi_pass : " << this->_cgi_pass << RESET << std::endl;
	std::cout << YELLOW << "_contentLocation : " << this->_contentLocation << RESET << std::endl;
	std::cout << YELLOW << "_path : " << this->_path << RESET << std::endl;


    if (location->getAutoindex() == "on")
        this->_isAutoIndex = true;
    else if (location->getAutoindex() == "off")
        this->_isAutoIndex = false;


    if (std::find(location->getAllowMethods().begin(), location->getAllowMethods().end(), request->getHttpMethodName()) == location->getAllowMethods().end())
		this->statusCode = 405;
	else if (atoi(location->getClientBodyBufferSize().c_str()) < static_cast<int>(request->getBody().size()))
		this->statusCode = 413;

    if (this->statusCode == 405 || this->statusCode == 413)
	{
		_response = notAllowed(location->getAllowMethods(), location->getPath(), this->statusCode, request->getAcceptLanguages().front().first) + "\r\n";
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
	header += "\r\n";
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

void			Response::GETmethod(Request* request, ServerScope* server)
{
	if (this->_cgi_pass != "")
	{
		Cgi	cgi(request, server, this->_path, this->_cgi_params);
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
		this->statusCode = readContent(server);
	else
		_response = this->readHtml(_errorMap[this->statusCode]);
	
	if (this->statusCode == 500)
		_response = this->readHtml(_errorMap[this->statusCode]);

	_response = getHeader(_response.size(), _path, this->statusCode, _type, this->_contentLocation, this->_contentLanguage) + "\r\n" + _response;
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
		_response = this->readHtml(_errorMap[this->statusCode]);
	_response = getHeader(_response.size(), _path, this->statusCode, _type, this->_contentLocation, this->_contentLanguage) + "\r\n" + _response;
}

void			Response::POSTmethod(Request* request, ServerScope* server)
{
	if (this->_cgi_pass != "")
	{
		Cgi	cgi(request, server, this->_path, _cgi_params);
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
		_response = this->readHtml(_errorMap[this->statusCode]);
	_response = getHeader(_response.size(), _path, this->statusCode, _type, this->_contentLocation, this->_contentLanguage) + "\r\n" + _response;
}

std::string		Response::readHtml(const std::string& path)
{
	std::ofstream		file;
	std::stringstream	buffer;

	if (pathIsFile(path))
	{
		file.open(path.c_str(), std::ifstream::in);
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

int				Response::readContent(ServerScope *server)
{
	std::ifstream		file;
	std::stringstream	buffer;

	_response = "";

	if (pathIsFile(_path))
	{
		file.open(_path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			_response = this->readHtml(_errorMap[403]);
			return (403);
		}

		buffer << file.rdbuf();
		_response = buffer.str();

		file.close();
	}
	else if (_isAutoIndex) 
	{
		buffer << getPage(_path.c_str(), server->getHost(), atoi(server->getPort().c_str()));
		_response = buffer.str();
		_type = "text/html";
	}
	else
	{
		_response = this->readHtml(_errorMap[404]);
		return (404);
	}

	return (200);
}


std::string		Response::getHeader(size_t size, const std::string& path, int code, std::string type, const std::string& contentLocation, const std::string& lang)
{
	std::string	header;

	resetValues();
	setValues(size, path, code, type, contentLocation, lang);

	header = "HTTP/1.1 " + std::to_string(code) + " " + getStatusMessage(code) + "\r\n";
	header += writeHeader();

	return (header);
}

std::string		Response::getStatusMessage(int code)
{
	if (_errorMap.find(code) != _errorMap.end())
		return _errorMap[code];
	return ("Unknown Code");
}

//auto_index ile ilgili
std::string         Response::getPage(const char *path, std::string const &host, int port) {
    std::string dirName(path);
    DIR *dir = opendir(path);
    std::string page =\
    "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
            <title>" + dirName + "</title>\n\
    </head>\n\
    <body>\n\
    <h1>INDEX</h1>\n\
    <p>\n";

    if (dir == NULL) {
        std::cerr << "Error: could not open [" << path << "]" << std::endl;
        return "";
    }
    if (dirName[0] != '/')
        dirName = "/" + dirName;
    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
        page += getLink(std::string(dirEntry->d_name), dirName, host, port);
    }
    page +="\
    </p>\n\
    </body>\n\
    </html>\n";
    closedir(dir);
    return page;
}
//auto_index ile ilgili
std::string         Response::getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host, int port) {
    std::stringstream   ss;
    ss << "\t\t<p><a href=\"http://" + host + ":" <<\
        port << dirName + "/" + dirEntry + "\">" + dirEntry + "</a></p>\n";
    return ss.str();
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
}