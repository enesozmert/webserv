#include "../inc/entity/Response.hpp"

Response::Response() {}

Response::~Response() {}

int Response::getStatusCode()
{
	return (this->statusCode);
}

std::string Response::getContentLocation()
{
	return (this->_contentLocation);
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

void Response::setContentType()
{
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

	this->_uri = this->_request->getPath();
	this->_serverRootPath = _serverScope->getRoot();
	this->_locationRootPath = _locationScope->getRoot();
	this->_index = selectIndex();
	if (_locationRootPath != "")
		this->_path = removeAdjacentSlashes(_locationRootPath + this->_uri);
	else if (_serverRootPath != "" && _locationRootPath == "")
		this->_path = removeAdjacentSlashes(_serverRootPath + this->_uri);
	this->_contentLocation = getPwd() + "/" + this->_path;
	if (!pathIsFile(this->_contentLocation))
	{
		if (_locationRootPath != "")
			this->_path = removeAdjacentSlashes(_locationRootPath + _index);
		else if (_serverRootPath != "" && _locationRootPath == "")
			this->_path = removeAdjacentSlashes(_serverRootPath + _index);
	}
	this->_contentLocation = this->_path;
	return 0;
}

void Response::setClientBodyBufferSize(std::string bodyBufferSize)
{
	this->_clientBodyBufferSize = atoi(bodyBufferSize.c_str());
}

void Response::setAutoIndex(std::string _autoIndex)
{
	if (_autoIndex == "on")
		this->_isAutoIndex = true;
	else if (_autoIndex == "off")
		this->_isAutoIndex = false;
}

void Response::setResponse(Request *request, ServerScope *server, LocationScope *location)
{
	this->statusCode = 200;
	this->_server = "webserv";
	this->_error_page = location->getErrorPage();
	this->_methodName = request->getHttpMethodName();
	this->_host = server->getHost();
	this->_port = atoi((server->getPort()).c_str());
	this->_contentLength = request->getContentLength();
	setAutoIndex(location->getAutoindex());
	setLanguage(request->getAcceptLanguages());
	setStaticErrorPage();
	this->_date = setDate();
	setAllowMethods(location->getAllowMethods());
	setIndexs(location->getIndex(), server->getIndex());
	setPaths();
	setContentType();
	setClientBodyBufferSize(location->getClientBodyBufferSize());
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

	setResponse(request, serverScope, locationScope);
	if (this->_clientBodyBufferSize != 0  && this->_clientBodyBufferSize < static_cast<int>(_request->getContentLength()))
	{
		this->statusCode = 403;
		_response = this->staticErrorPage[403];
		_response = getHeader() + _response;
		writeResponse();
		return _response;
	}
	if (std::find(_allow_methods.begin(), _allow_methods.end(), this->_methodName) == _allow_methods.end())
	{
		this->statusCode = 405;
		_response = notAllowed();
		return _response;
	}
	selectCgiPass();
	handleMethods();
	_response = getHeader() + _response;
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
	size_t i = 0;
	size_t j = _response.size() - 2;

	_response = this->executeCgi();
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
	if (this->_methodName == "DELETE" && this->_cgiPass == "")
	{
		if (this->_methodName == "DELETE")
			deleteMethod();
		return;
	}
	else if (this->_methodName == "GET" && this->_cgiPass == "")
	{
		if (this->statusCode != 200)
			_response = this->errorHtml();
		else
			readContent();
		return ;
	}
	else if (this->_cgiPass != "" && (this->_methodName == "POST" || this->_methodName == "GET" || this->_methodName == "DELETE"))
	{
		handleCgi();
		if ((this->_methodName == "GET" || this->_methodName == "POST" || this->_methodName == "DELETE") && this->statusCode != 200)
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
	std::stringstream ss(this->_body);
    std::string filePath = "";
	std::string delPath = "";

	std::string token;
    while (std::getline(ss, token, ':')) {
        size_t startPos = token.find_first_of('"');
        size_t endPos = token.find_last_of('"');
        if (startPos != std::string::npos && endPos != std::string::npos) {
            filePath = token.substr(startPos + 1, endPos - startPos - 1);
        }
    }
	delPath = getPwd() + "/" + this->_locationRootPath + filePath;
	if (pathIsFile(delPath))
	{
		if (remove(delPath.c_str()) == 0)
		{
			this->statusCode = 200;
			this->_response = "File Deletion Successful";
		}
		else
			this->statusCode = 403;
	}
	else
		this->statusCode = 404;

	if (this->statusCode == 403 || this->statusCode == 404)
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
		header += it->getName() + ": " + *it->getValue() + "\n";
	}
	header += "\n";
	return (header);
}

std::string Response::getHeader()
{
	std::string header;

	setContentType();
	this->_status = "close";
	this->_contentLength = to_string(this->_response.size());
	header = "HTTP/1.1 " + to_string(this->statusCode) + " " + _httpStatusCode.getByStatusCode(this->statusCode).getValue() + "\r\n";
	header += writeHeader();

	return (header);
}

void Response::keywordFill()
{
	_keywordDatabase.insertData(Variable<std::string>("Date", &this->_date));
	_keywordDatabase.insertData(Variable<std::string>("Server", &this->_server));
	_keywordDatabase.insertData(Variable<std::string>("Content-Type", &this->_contentType));
	_keywordDatabase.insertData(Variable<std::string>("Content-Length", &this->_contentLength));
	_keywordDatabase.insertData(Variable<std::string>("Connection", &this->_status));
	_keywordDatabase.insertData(Variable<std::string>("Content-Language", &this->_contentLanguage));
	_keywordDatabase.insertData(Variable<std::string>("Content-Location", &this->_contentLocation));
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

	for (size_t i = 0; i < _locationScope->getCgiPass().size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (_locationScope->getCgiPass()[i].find(cgiNames[j]) != std::string::npos && cgiExtension == cgiExtensions[j])
			{
				this->_cgiPass = _locationScope->getCgiPass()[i];
				return;
			}
		}
	}
	this->_cgiPass = "";

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
		std::cout << GREEN << "\rResponse :                " << std::endl << "[" << _response << RESET << "]\n" << std::endl;
	else
		std::cout << GREEN << "\rResponse :                " << std::endl << "[" << _response.substr(0, 500) << "..." << "]\n" << RESET << std::endl;
}



/*******CGI*********/

std::string Response::executeCgi()
{
	std::cout << GREEN << "/*******CGI*********/" << RESET << std::endl;
	this->keywordFillCgi();

	char	output[4096];
	int		readed;
	int	body_pipe[2];
	int	result_pipe[2];
	std::string tmp;
	std::string tmp2;
	char *av1 = (char *)this->_cgiPass.c_str();
	char *av2;
	char *av[3];
	char cwd[4096];

	getcwd(cwd, 4096);

	av[2] = 0;

	tmp = (std::string)cwd + "/" + this->_path;
	av2 = (char *)tmp.c_str();

	av[0] = av1;
	av[1] = av2;

	char **env = mapToEnvForm(this->_envDatabase.getAllData());
	for (int i = 0; env[i] != NULL; ++i) {
        std::cout << env[i] << std::endl;
    }

	pipe(body_pipe);
	pipe(result_pipe);
	if (this->_methodName == "POST" || this->_methodName == "DELETE")
		write(body_pipe[1], _body.c_str(), _body.length());

	close(body_pipe[1]);

	if (!fork())
	{

		close(result_pipe[0]);
		dup2(result_pipe[1], 1);
		close(result_pipe[1]);

		if (this->_methodName == "POST" || this->_methodName == "DELETE")
			dup2(body_pipe[0], 0);
		close(body_pipe[0]);

		execve(av[0], av, env);
		std::cout << "Execv Err!" << std::endl << std::flush;
		this->statusCode = 500;
		exit(-1);
	}
	wait(NULL);
	close(body_pipe[0]);
	close(result_pipe[1]);

	readed = read(result_pipe[0], output, 4096);
	if (readed == 0)
		std::cout << "Cgi Read Fail!" << std::endl << std::flush;
	close(result_pipe[0]);
	output[readed] = 0;

	for (int i = 0; env[i]; i++)
		free(env[i]);
	free(env);

	return (std::string(output, readed));
}



DataBase<CgiVariable<std::string, std::string> > Response::getEnvDataBase()
{
	return (this->_envDatabase);
}

void Response::setEnvDatabase(DataBase<CgiVariable<std::string, std::string> > envDatabase)
{
	this->_envDatabase = envDatabase;
}

void Response::keywordFillCgi()
{
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_FILENAME", this->_path));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_NAME", this->_path));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_TYPE", this->_request->getContentType()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_LENGTH", to_string(this->_request->getContentLength())));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("PATH_INFO", this->_request->getPath()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_METHOD", this->_methodName));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_URI", this->_uri));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PORT", to_string(this->_port)));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_SOFTWARE", "webserv"));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REDIRECT_STATUS", "200"));
}