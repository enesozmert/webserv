#include "../inc/entity/Response.hpp"

Response::Response() {}

Response::~Response() {}

Response::Response(const Response &response)
{
	*this = response;
}

Response &Response::operator=(const Response &response)
{
	if (this == &response)
		return (*this);
	this->_response = response._response;
	return (*this);
}

int Response::getStatusCode()
{
	return (this->statusCode);
}

std::string Response::getContentLocation()
{
	return (this->_contentLocation);
}
std::string Response::getContentDisposition()
{
	return (this->_contentDisposition);
}

std::string Response::getContentType()
{
	return (this->_contentType);
}

std::string Response::getPath()
{
	return (this->_path);
}

std::string Response::getServerName()
{
	return (this->_server);
}

std::map<std::string, std::string> Response::getQueries()
{
	return (this->_queries);
}

std::string Response::getResponse()
{
	return (this->_response);
}

std::string Response::getCgiPass()
{
	return (this->_cgiPass);
}

std::string Response::getBody()
{
	return (this->_body);
}

std::string Response::getMethodName()
{
	return (this->_methodName);
}

DataBase<Variable<std::string> > Response::getKeywordDataBase()
{
	return (this->_keywordDatabase);
}

void Response::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
	this->_keywordDatabase = keywordDatabase;
}

void Response::setStaticErrorPage()
{
	this->staticErrorPage.clear();
	this->staticErrorPage[204] = "<!DOCTYPE html>\n<html><title>204</title><body>request has succeeded, but that the client doesn't need to navigate away from its current page</body></html>\n";
	this->staticErrorPage[403] = "<!DOCTYPE html>\n<html><title>403</title><body>This request cannot be authorized (invalid permissions or credentials)</body></html>\n";
	this->staticErrorPage[404] = "<!DOCTYPE html>\n<html><title>404</title><body>There was an error finding your error page</body></html>\n";
	this->staticErrorPage[500] = "<!DOCTYPE html>\n<html><title>500</title><body>Server couldn't handle your request. Still, you won't kill it so easily !</body></html>\n";
}

void Response::setAllowMethods(std::vector<std::string> methods)
{

	this->_allow_methods = methods;

	std::vector<std::string>::iterator it = methods.begin();

	while (it != methods.end())
	{
		this->_allows += *(it++);

		if (it != methods.end())
			this->_allows += ", ";
	}
}

void Response::setQueries()
{
	/* const std::size_t pair_delimiter = _body.find('=');
	const std::string key = _body.substr(0, pair_delimiter);
	std::string value = _body.substr(pair_delimiter + 1, _body.length() - (pair_delimiter + 1));
	this->_queries[key] = value; */
	std::size_t position = 0;
	while (position < _body.size())
	{
		const std::size_t next_delimiter = _body.find('&', position);
		std::string query;
		if (next_delimiter == std::string::npos)
		{
			query = _body.substr(position);
		}
		else
		{
			query = _body.substr(position, next_delimiter - position);
		}
		const std::size_t pair_delimiter = query.find('=');
		const std::string name = query.substr(0, pair_delimiter);
		if (name.empty())
		{
			return;
		}
		std::string value;
		if (pair_delimiter != std::string::npos)
		{
			value = query.substr(pair_delimiter + 1);
		}
		this->_queries[name] = value;
		if (next_delimiter == std::string::npos)
		{
			break;
		}
		position = next_delimiter + 1;
	}
}

void Response::setContentType()
{
/* 	if (_body.find("Content-Type: ") != std::string::npos)
	{
		_contentType = "image/png";
		return ;
	} */
	if (cgi_return_type != "")
	{
		_contentType = cgi_return_type;
		return;
	}
	else
	{
		this->_type = this->_path.substr(this->_path.rfind(".") + 1, this->_path.size() - this->_path.rfind("."));
		this->_contentType = _httpContentType.contentTypeGenerator(trim(this->_type, "\n\r\t "));
	}
	std::cout << "Content-Type: " << _contentType << std::endl;
}

void Response::setLastModified()
{
	char buffer[100];
	struct stat stats;
	struct tm *tm;

	if (stat(this->_path.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		_lastModified = std::string(buffer);
	}
}

void Response::setIndexs(std::vector<std::string> _locationIndex, std::vector<std::string> _serverIndex)
{
	for (std::vector<std::string>::iterator it = _locationIndex.begin(); it != _locationIndex.end(); it++)
		this->_indexs.push_back(*it);
	for (std::vector<std::string>::iterator itt = _serverIndex.begin(); itt != _serverIndex.end(); itt++)
		this->_indexs.push_back(*itt);
}

void Response::setLanguage(std::vector<std::pair<std::string, float> > languages)
{

	for (size_t i = 0; i < languages.size(); i++)
	{
		if (!this->_contentLanguage.empty())
		{
			this->_contentLanguage += ", ";
		}
		this->_contentLanguage += languages[i].first;
	}
}

int Response::setPaths()
{
	std::string trimmed;

	this->_serverRootPath = _serverScope->getRoot();
	this->_locationRootPath = _locationScope->getRoot();
	this->_index = selectIndex();

	if (_locationRootPath != "")
		this->_path = removeAdjacentSlashes(getPwd() + "/" + _locationRootPath + _request->getPath());
	else if (_serverRootPath != "" && _locationRootPath == "")
		this->_path = removeAdjacentSlashes(getPwd() + "/" + _serverRootPath + _request->getPath());

	if (!pathIsFile(this->_path))
	{
		if (_locationRootPath != "")
			this->_path = removeAdjacentSlashes(getPwd() + "/" + _locationRootPath + _index);
		else if (_serverRootPath != "" && _locationRootPath == "")
			this->_path = removeAdjacentSlashes(getPwd() + "/" + _serverRootPath + _index);
	}
	this->_contentLocation = this->_path;
	std::cout << "this->_contentLocation " << this->_contentLocation << std::endl;
	return 0;
}

void Response::setClientBodyBufferSize(std::string bodyBufferSize)
{
	this->_clientBodyBufferSize = atoi(bodyBufferSize.c_str());
	if (this->_clientBodyBufferSize == 0)
		this->_clientBodyBufferSize = _request->getContentLength();
}

void Response::setAutoIndex(std::string _autoIndex)
{
	if (_autoIndex == "on")
		this->_isAutoIndex = true;
	else if (_autoIndex == "off")
		this->_isAutoIndex = false;
}

int Response::setResponse(Request *request, ServerScope *server, LocationScope *location)
{
	this->statusCode = 200;
	this->_server = "webserv";
	this->_error_page = location->getErrorPage();
	this->_methodName = request->getHttpMethodName();
	this->_host = server->getHost();
	this->_port = atoi((server->getPort()).c_str());
	setAutoIndex(location->getAutoindex());
	//setLanguage(request->getAcceptLanguages());
	setStaticErrorPage();
	this->_date = setDate();
	setLastModified();
	setAllowMethods(location->getAllowMethods());
	setIndexs(location->getIndex(), server->getIndex());
	setPaths();
	setContentType();
	setClientBodyBufferSize(location->getClientBodyBufferSize());
	if (this->_methodName == "GET")
		setQueries();
	return 0;
}

std::string Response::createResponse(Request *request, ServerScope *serverScope, LocationScope *locationScope, std::string MultiBody)
{
	this->_request = request;
	this->_serverScope = serverScope;
	this->_locationScope = locationScope;
	if (MultiBody != "")
		this->_body = MultiBody;
	else
		this->_body = this->_request->getBody();

	if (setResponse(request, serverScope, locationScope) == -1)
	{
		std::cerr << RED << "Error setting response" << RESET << std::endl;
		return NULL;
	}

	/* if (std::find(_allow_methods.begin(), _allow_methods.end(), this->_methodName) == _allow_methods.end())
	{
		this->statusCode = 405;
		_response = notAllowed() + "\r\n";
		return -1;
	}
	else if (this->_clientBodyBufferSize < static_cast<int>(this->_body.size()))
	{
		this->statusCode = 413;
		_response = notAllowed() + "\r\n";
		return -1;
	} */
	selectCgiPass();
	handleMethods();
	_response = getHeader() + "\r\n" + _response;
	writeResponse();
	return _response;
}

std::string Response::notAllowed()
{
	std::string header;

	_response = "";
	this->_contentLength = "";
	if (this->statusCode == 405)
		header = "HTTP/1.1 405 " + _httpStatusCode.getByStatusCode(405).getValue() + "\r\n";
	else if (this->statusCode == 413)
		header = "HTTP/1.1 413 " + _httpStatusCode.getByStatusCode(413).getValue() + "\r\n";
	header += writeHeader();
	return (header);
}

void Response::handleCgi()
{
	Cgi cgi(_request, this, _serverScope, _locationScope);
	size_t i = 0;
	size_t j = _response.size() - 2;

	_response = cgi.executeCgi(this->_cgiPass);
	while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
	{
		std::string str = _response.substr(i, _response.find("\r\n", i) - i);
		if (str.find("Status: ") == 0)
			this->statusCode = std::atoi(str.substr(8, 3).c_str());
		else if (str.find("Content-type: ") == 0)
			this->cgi_return_type = str.substr(14, str.size());
		i += str.size() + 2;
	}
	while (_response.find("\r\n", j) == j)
		j -= 2;
	_response = _response.substr(i, j - i);
}

void Response::handleMethods()
{
	if (this->_methodName == "DELETE")
	{
		if (this->_methodName == "DELETE")
			deleteMethod();
		return;
	}
	else if (this->_methodName == "GET")
	{
		if (this->statusCode != 200)
			_response = this->errorHtml();
		else
			readContent();
		return ;
	}
	else if (this->_cgiPass != "" && this->_methodName == "POST")
	{
		handleCgi();
		if (this->_methodName == "POST" && this->statusCode != 200)
		{
			_response = this->errorHtml();
		}
		return ;
	}
	else
	{
		this->statusCode = 204;
		_response = "";
		return ;
	}
}

void Response::deleteMethod()
{
	std::cout << PURPLE << "******DELETE******" << RESET << std::endl;
	_response = "";
	this->_path = realpath(".", NULL) + _path;
	std::cout << PURPLE << "**path***" << _path << RESET << std::endl;
	if (pathIsFile(_path))
	{
		if (remove(_path.c_str()) == 0)
			this->statusCode = 204;
		else
			this->statusCode = 403;
	}
	else
		this->statusCode = 404;

	if (this->statusCode == 403 || this->statusCode == 404 || this->statusCode == 204)
		_response = this->errorHtml();
}

std::string Response::errorHtml()
{
	std::ofstream file;
	std::stringstream buffer;

	if (this->statusCode == 403)
		return (staticErrorPage[403]);
	else if (this->statusCode == 500)
		return (staticErrorPage[500]);
	else if (this->statusCode == 204)
		return (staticErrorPage[204]);

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

void Response::readContent()
{
	std::ifstream file;
	std::stringstream buffer;

	if (pathIsFile(_path))
	{
		file.open(_path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			this->statusCode = 403;
			_response = staticErrorPage[403];
			return;
		}
		buffer << file.rdbuf();
		_response = buffer.str();
		file.close();
		return;
	}
	else if (_isAutoIndex)
	{
		buffer << getPage();
		_response = buffer.str();
		_type = "text/html";
	}
	else
	{
		this->statusCode = 404;
		_response = this->errorHtml();
	}
	return;
}

std::string Response::writeHeader(void)
{
	std::string header = "";
	this->keywordFill();
	std::vector<Variable<std::string> > vec1 = this->_keywordDatabase.getAllData();
	for (std::vector<Variable<std::string> >::iterator it = vec1.begin(); it != vec1.end(); ++it)
	{
		header += it->getName() + ": " + *it->getValue() + "\n\r";
	}
	header += "\r\n";
	return (header);
}

std::string Response::getHeader()
{
	std::string header;

	//setContentType();
	this->_contentType = "text/html";
	this->_contentLength = to_string(this->_response.size());
	header = "HTTP/1.1 " + to_string(this->statusCode) + " " + _httpStatusCode.getByStatusCode(this->statusCode).getValue() + "\r\n";
	header += writeHeader();

	return (header);
}

void Response::keywordFill()
{
	_keywordDatabase.insertData(Variable<std::string>("Allow", &this->_allows));
	_keywordDatabase.insertData(Variable<std::string>("Content-Language", &this->_contentLanguage));
	_keywordDatabase.insertData(Variable<std::string>("Content-Length", &this->_contentLength));
	_keywordDatabase.insertData(Variable<std::string>("Content-Location", &this->_contentLocation));
	_keywordDatabase.insertData(Variable<std::string>("Content-Type", &this->_contentType));
	_keywordDatabase.insertData(Variable<std::string>("Date", &this->_date));
	// _keywordDatabase.insertData(Variable<std::string>("Last-Modified", &this->_lastModified));
	_keywordDatabase.insertData(Variable<std::string>("Server", &this->_server));
	//_keywordDatabase.insertData(Variable<std::string>("Content-Disposition", &this->_contentDisposition));
}

std::string Response::selectIndex()
{
	for (std::vector<std::string>::iterator it = this->_indexs.begin(); it != this->_indexs.end(); it++)
	{
		if (!pathIsFile(*it))
			return (*it);
	}
	std::cerr << RED << "No index found" << RESET << std::endl;
	return NULL;
}

void Response::selectCgiPass()
{
	std::string cgiExtensions[3] = {"py", "pl", "php"};
	std::string cgiNames[3] = {"python", "perl", "php"};
	std::string cgiExtension = this->_path.substr(this->_path.find(".") + 1, this->_path.length());
	//this->_cgiPass = "/usr/bin/php-cgi";
	this->_cgiPass = getPwd() + "/" + "cgi_tester";
	// this->_cgiPass = getPwd() + "/" + "cgi_tester";
	/* if (cgiExtension == "php" || cgiExtension == "pl" || cgiExtension == "py")
		this->_cgiPass = getPwd() + "/" + "cgi_tester";
	else
		this->_cgiPass = "";
	std::cout << PURPLE << "this->_cgiPass " << RESET << this->_cgiPass << std::endl; */
/* 	for (size_t i = 0; i < _locationScope->getCgiPass().size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (_locationScope->getCgiPass()[i].find(cgiNames[j]) != std::string::npos && cgiExtension == cgiExtensions[j])
			{
				this->_cgiPass = _locationScope->getCgiPass()[i];
				std::cout << PURPLE << "this->_cgiPass " << RESET << this->_cgiPass << std::endl;
				return;
			}
		}
	}
	this->_cgiPass = "";
	this->_cgiPass = "/opt/homebrew/bin/php-cgi";
	std::cout << CYAN << "this->_cgiPass " << RESET << this->_cgiPass << std::endl;*/
}

/********->auto_index<-*******/

std::string Response::getPage()
{
	std::string dirName(_locationRootPath.c_str());
	DIR *dir = opendir(_locationRootPath.c_str());
	std::string page =
		"<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
            <title>" +
		dirName + "</title>\n\
    </head>\n\
    <body>\n\
    <h1>INDEX</h1>\n\
    <p>\n";

	if (dir == NULL)
	{
		std::cerr << RED << "Error: could not open [" << _locationRootPath << "]" << RESET << std::endl;
		return "";
	}
	if (dirName[0] != '/')
		dirName = "/" + dirName;
	for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
	{
		page += Response::getLink(std::string(dirEntry->d_name), dirName);
	}
	page += "\
    </p>\n\
    </body>\n\
    </html>\n";
	closedir(dir);
	return page;
}

std::string Response::getLink(std::string const &dirEntry, std::string const &dirName)
{
	std::stringstream ss;
	ss << "\t\t<p><a href=\"http://" + this->_host << dirName + dirEntry + "\">" + dirEntry + "</a></p>\n";
	return ss.str();
}

void Response::writeResponse()
{
	if (_response.size() < 500)
		std::cout << GREEN << "\rResponse :                " << std::endl << "[" << _response << RESET << "]\n" << RESET << std::endl;
	else
		std::cout << GREEN << "\rResponse :                " << std::endl << "[" << _response.substr(0, 500) << "..." << "]\n" << RESET << std::endl;
}
