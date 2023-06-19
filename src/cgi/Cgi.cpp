#include "../inc/cgi/Cgi.hpp"

Cgi::Cgi() {}

Cgi::~Cgi() {}

Cgi::Cgi(const Cgi &cgi)
{
	*this = cgi;
}

Cgi &Cgi::operator=(const Cgi &cgi)
{
	if (this == &cgi)
		return (*this);
	this->_body = cgi._body;
	return (*this);
}

Cgi::Cgi(Request *request, Response *response,  ServerScope *serverScope, LocationScope *locationScope) : 
	_request(request), _response(response), _serverScope(serverScope), _locationScope(locationScope), _body(response->getBody())
{
	//this->_query = _response->getQueries();
	(void)_locationScope;
	keywordFill();
	env = mapToEnvForm(this->_envDatabase.getAllData());
	for (size_t i = 0; env[i] != NULL; i++)
	{
		std::cout << "env[]" << env[i] << std::endl;
	}
	
}

std::string Cgi::executeCgi(std::string scriptName)
{
	int readed;
	char output[4096];
	int pipeFd[2];
	int pipeo[2];

	if (pipe(pipeFd) == -1)
		return "Status: 500\r\n\r\n";
	if (pipe(pipeo) == -1)
		return "Status: 500\r\n\r\n";


	//std::cout << RED << "_body: " << _body << std::endl;
	if (write(pipeFd[1], _body.c_str(), _body.length()) == -1)
	{
		std::cerr << RED << "Write problem" << RESET << std::endl;
	}


	close(pipeFd[1]);
	std::string contentLocation = _response->getContentLocation();
	std::cout << GREEN << "contentLocation = " << contentLocation << RESET << std::endl;
	char *cmd[] = {&scriptName[0], &contentLocation[0], NULL};
	if (!fork())
	{

		close(pipeo[0]);
		dup2(pipeo[1], 1);
		close(pipeo[1]);

		dup2(pipeFd[0], 0);
		close(pipeFd[0]);

		execve(cmd[0], cmd, env);
		std::cout << "Execv Err!" << std::endl
				  << std::flush;
		exit(-1);
	}
	wait(NULL);
	close(pipeFd[0]);
	close(pipeo[1]);

	readed = read(pipeo[0], output, 4096);
	if (readed == 0)
		std::cout << "Cgi Read Fail!" << std::endl
				  << std::flush;
	close(pipeo[0]);
	output[readed] = 0;
	std::cout << "output : " << output << std::endl;
	return (std::string(output, readed));
}

DataBase<CgiVariable<std::string, std::string> > Cgi::getEnvDataBase()
{
	return (this->_envDatabase);
}

void Cgi::setEnvDatabase(DataBase<CgiVariable<std::string, std::string> > envDatabase)
{
	this->_envDatabase = envDatabase;
}

void Cgi::keywordFill()
{
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_FILENAME", _response->getContentLocation()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_NAME", _response->getContentLocation()));
	if (_request->getContentType() == "")
		_envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_TYPE", "text/html"));
	else
		_envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_TYPE", _request->getContentType()));
	
	_envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_LENGTH", to_string(_request->getContentLength())));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("PATH_INFO", _request->getPath()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
	//_envDatabase.insertData(CgiVariable<std::string, std::string>("QUERY_STRING", request.getQuery()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_METHOD", _response->getMethodName()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_URI", removeAdjacentSlashes(_request->getPath())));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PORT", _serverScope->getPort()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_SOFTWARE", "webserv"));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REDIRECT_STATUS", "200"));

/* 	if (_response->getMethodName() == "GET")
	{
		for (std::map<std::string, std::string>::iterator it = _query.begin(); it != _query.end(); it++)
		{
			_envDatabase.insertData(CgiVariable<std::string, std::string>(it->first, it->second));
			std::cout << CYAN << it->first << "=" << it->second << RESET << std::endl;
		}
	} */
}
