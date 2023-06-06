#include "../inc/entity/Response.hpp"

Response::Response() {}

Response::~Response() {}

Response::Response(const Response &response)
{
	*this = response;
}

Response& Response::operator=(const Response &response)
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

std::string Response::getPath()
{
	return (this->_path);
}

std::string Response::getServerName()
{
	return (this->_server);
}

std::string Response::getResponse()
{
	return (this->_response);
}

std::string Response::getCgiPass()
{
	return (this->_cgi_pass);
}

std::string Response::getBody()
{
	return (this->_body);
}

std::string Response::getRaw()
{
	return (this->_raw);
}


std::string Response::getMethod()
{
	return (this->_method);
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

void Response::setContentType()
{
	if (_type != "")
	{
		_contentType = _type;
		return ;
	}
	this->_type = this->_path.substr(this->_path.rfind(".") + 1, this->_path.size() - this->_path.rfind("."));
	this->cgiType = trim(this->_type, "\n\r\t ");//pl, php, py
	this->_contentType = _httpContentType.contentTypeGenerator(trim(this->_type, "\n\r\t "));
	std::cout << "this->cgiType = " << this->cgiType << std::endl;
	std::cout << "this->_type = " << this->_type << std::endl;
	std::cout << "this->_contentType = " <<  this->_contentType << std::endl;
}

void Response::setDate()
{
	char buffer[100];
	struct timeval tv;
	struct tm *tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	_date = std::string(buffer);
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
	//_locationIndex ve _serverIndex ayrı ayrı vector olarak al
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

int Response::setPaths(ServerScope *server, LocationScope *location, std::string path)
{
	this->_serverRootPath = server->getRoot();
	this->_locationRootPath = location->getRoot();
	this->_index = selectIndex();
	std::string trimmed;
	trimmed = trim(path, "\n\r\t ");

	if (trimmed == "/favicon.ico" || trimmed == "favicon.ico")
		this->_path = "/tests/test1/icon.png";

	if (_locationRootPath != "")
		this->_path = removeAdjacentSlashes(_locationRootPath + trimmed);
	else if (_serverRootPath != "" && _locationRootPath == "")
		this->_path = removeAdjacentSlashes(_serverRootPath + trimmed);

	if (!pathIsFile(this->_path))//gelen path ile eşleşen bir dosya yoksa 0 döner ve içeri girer.
	{
		if (_locationRootPath != "")
			this->_path = removeAdjacentSlashes(_locationRootPath + _index);
		else if (_serverRootPath != "" && _locationRootPath == "")
			this->_path = removeAdjacentSlashes(_serverRootPath + _index);
	}
	this->_contentLocation = removeAdjacentSlashes(getPwd() + "/" + this->_path);
	std::cout << YELLOW << "_contentLocation : " << this->_contentLocation << RESET << std::endl;
	std::cout << YELLOW << "_path : " << this->_path << RESET << std::endl;
	return 0;
}

void Response::setClientBodyBufferSize(std::string bodyBufferSize)
{
	this->_clientBodybufferSize = atoi(bodyBufferSize.c_str());
}

void Response::setAutoIndex(std::string _autoIndex)
{
	if (_autoIndex == "on")
		this->_isAutoIndex = true;
	else if (_autoIndex == "off")
		this->_isAutoIndex = false;

	std::cout << "auto_index : " << _isAutoIndex << std::endl;
}

int Response::setResponse(Request *request, ServerScope *server, LocationScope *location)
{
	this->_cgi_pass = "";
	std::cout << YELLOW << "request.getPath() : " << request->getPath() << RESET << std::endl;
	this->statusCode = request->getReturnCode(); // statusCode 200 olarak initledik. İlk 200 olarak atanacak.
	this->_body = request->getBody();
	this->_server = "webserv";
	this->_error_page = location->getErrorPage();
	this->_method = request->getHttpMethodName();
	this->_host = server->getHost();
	this->_port = atoi((server->getPort()).c_str());
	setAutoIndex(location->getAutoindex());
	setLanguage(request->getAcceptLanguages());
	std::cout << YELLOW << "_LANGUAGE : " << this->_contentLanguage << RESET << std::endl;
	setStaticErrorPage();
	setDate();
	setLastModified();
	setAllowMethods(location->getAllowMethods());
	setIndexs(location->getIndex(), server->getIndex()); // index yoksa hata mı vermeli?
	setPaths(server, location, request->getPath());
	setContentType();
	setClientBodyBufferSize(location->getClientBodyBufferSize());
	if(this->cgiType == "pl" || this->cgiType == "py" || this->cgiType == "php" )
		this->_cgi_pass = location->getCgiPass();
	return 0;
}

void Response::createResponse(Request *request, ServerScope *server, LocationScope *location, std::string raw)
{
	this->_raw = raw;
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

void Response::GET_method(Request *request, ServerScope *server)
{
	if (this->_cgi_pass != "")
	{
		std::cout << PURPLE << "******Cgi_GET******" << RESET << std::endl;
		Cgi cgi(request, server, this);
		size_t i = 0;
		size_t j = _response.size() - 2;

		_response = cgi.executeCgi(this->_cgi_pass);
		while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
		{
			std::string str = _response.substr(i, _response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				this->statusCode = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-type: ") == 0)
				this->_type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (_response.find("\r\n", j) == j)
			j -= 2;
		_response = _response.substr(i, j - i);
	}
	else if (this->statusCode == 200)
		readContent();
	else
		_response = this->errorHtml();

	if (this->statusCode == 500)
		_response = staticErrorPage[500];

	_response = getHeader() + "\r\n" + _response;
}

void Response::DELETE_method()
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
		_response = this->errorHtml();

	_response = getHeader() + "\r\n" + _response;
}

void Response::POST_method(Request *request, ServerScope *server)
{
	if (this->_cgi_pass != "")
	{
		std::cout << PURPLE << "******Cgi_POST*****" << RESET << std::endl;
		Cgi cgi(request, server, this);
		size_t i = 0;
		size_t j = _response.size() - 2;

		_response = cgi.executeCgi(this->_cgi_pass);
		while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
		{
			std::string str = _response.substr(i, _response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				this->statusCode = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-Type: ") == 0)
				this->_type = str.substr(14, str.size());
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

std::string Response::errorHtml()
{
	std::ofstream file;
	std::stringstream buffer;

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
		std::cout << "buraya girdi" << std::endl;
		buffer << file.rdbuf();
		_response = buffer.str();
		file.close();
		return;
	}
	else if (_isAutoIndex) {
		buffer << getPage();
		_response = buffer.str();
		_type = "text/html";
	}
	else{
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
	for(std::vector<Variable<std::string> >::iterator it = vec1.begin(); it != vec1.end(); ++it)
	{
		header += it->getName() + ": " + *it->getValue() + "\n\r";
	}
	header += "\r\n";
	return (header);
}

std::string Response::getHeader()
{
	std::string header;
	
	std::cout << "std::to_string(this->_response.size()) : " << std::to_string(this->_response.size()) << std::endl;
	this->_contentLength = std::to_string(this->_response.size());
	header = "HTTP/1.1 " + std::to_string(this->statusCode) + " " + _httpStatusCode.getByStatusCode(this->statusCode).getValue() + "\r\n";
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
    _keywordDatabase.insertData(Variable<std::string>("Last-Modified", &this->_lastModified));
    _keywordDatabase.insertData(Variable<std::string>("Server", &this->_server));
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

/********->auto_index<-*******/

std::string         Response::getPage()
{
    std::string dirName(_locationRootPath.c_str());
    DIR *dir = opendir(_locationRootPath.c_str());
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
        std::cerr << RED << "Error: could not open [" << _locationRootPath << "]" << RESET << std::endl;
        return "";
    }
    if (dirName[0] != '/')
        dirName = "/" + dirName;
    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
        page += Response::getLink(std::string(dirEntry->d_name), dirName);
    }
    page +="\
    </p>\n\
    </body>\n\
    </html>\n";
    closedir(dir);
    return page;
}

std::string         Response::getLink(std::string const &dirEntry, std::string const &dirName) {
    std::stringstream   ss;
    ss << "\t\t<p><a href=\"http://" + this->_host << dirName + dirEntry + "\">" + dirEntry + "</a></p>\n";
    return ss.str();
}
