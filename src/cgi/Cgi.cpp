#include "../inc/cgi/Cgi.hpp"

/* Cgi::Cgi() {}

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
} */

<<<<<<< HEAD
Cgi::Cgi(Request *request, Response *response,  ServerScope *serverScope, LocationScope *locationScope) : 
	_request(request), _response(response), _serverScope(serverScope), _locationScope(locationScope), _body(response->getBody())
=======
Cgi::Cgi(std::string methodName, std::string body, std::string path, int port, std::string contentLength, std::string contentType)
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
{
	//this->_query = _response->getQueries();
	this->_methodName = methodName;
	this->_body = body;
	this->_path = path;
	this->_port = port;
	this->_contentLength = contentLength;
	this->_contentType = contentType;
	keywordFill();
<<<<<<< HEAD
=======
/* 	for (size_t i = 0; env[i] != NULL; i++)
	{
		std::cout << "env[]" << env[i] << std::endl;
	} */
	
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
}

std::string Cgi::executeCgi(std::string scriptName)
{
<<<<<<< HEAD
	int readed;
	char output[4096];
	int pipeFd[2];
	int pipeo[2];
	char **env = NULL;

	env = mapToEnvForm(this->_envDatabase.getAllData());
	for (size_t i = 0; env[i] != NULL; i++)
	{
		std::cout << "env[i] : " << env[i] << std::endl;
	}
	
	if (pipe(pipeFd) == -1)
		return "Status: 500\r\n\r\n";
	if (pipe(pipeo) == -1)
		return "Status: 500\r\n\r\n";

	if (_request->getHttpMethodName().find("POST") != std::string::npos)
	{
		if (write(pipeFd[1], _body.c_str(), _body.size()) == -1)
		{
			std::cerr << RED << "Write problem" << RESET << std::endl;
		}
	}

	close(pipeFd[1]);
	std::string contentLocation = _response->getContentLocation();
	char *cmd[] = {&scriptName[0], &contentLocation[0], NULL};
=======
	char	output[4096];
	int		readed;
	int	pipeFd[2];
	int	pipeo[2];
	std::string tmp;
	std::string tmp2;
	char *av1 = (char *)scriptName.c_str();
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

	pipe(pipeFd);
	pipe(pipeo);
	if (this->_methodName == "POST")
		write(pipeFd[1], _body.c_str(), _body.length());

	close(pipeFd[1]);

>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
	if (!fork())
	{

		close(pipeo[0]);
		dup2(pipeo[1], 1);
		close(pipeo[1]);

		if (this->_methodName == "POST")
			dup2(pipeFd[0], 0);
		close(pipeFd[0]);

<<<<<<< HEAD
		execve(cmd[0], cmd, env);
		std::cout << "Execv Err!" << std::endl
				  << std::flush;
		exit(-1);
	}
	waitpid(-1, NULL, 0);
=======
		execve(av[0], av, env);
		std::cout << "Execv Err!" << std::endl << std::flush;
		exit(-1);
	}
	wait(NULL);
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
	close(pipeFd[0]);
	close(pipeo[1]);

	readed = read(pipeo[0], output, 4096);
	if (readed == 0)
		std::cout << "Cgi Read Fail!" << std::endl
				  << std::flush;
	close(pipeo[0]);
	output[readed] = 0;
<<<<<<< HEAD
	return (std::string(output, readed));
}
=======

	for (int i = 0; env[i]; i++)
		free(env[i]);
	free(env);

	std::cout << "std::string(output, readed)\n" << std::string(output, readed) << std::endl;
	return (std::string(output, readed));
}


>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61

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
<<<<<<< HEAD
	// _envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_NAME", _response->getContentLocation()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_FILENAME", _locationScope->getRoot() + "/" + _locationScope->getIndex()[0]));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_TYPE", _request->getContentType()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_LENGTH", to_string(_request->getContentLength())));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("PATH_INFO", _locationScope->getIndex()[0]));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_METHOD", _response->getMethodName()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_URI", _locationScope->getIndex()[0]));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REMOTEaddr", _serverScope->getHost()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PORT", _serverScope->getPort()));
=======
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_FILENAME", this->_path));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_NAME", this->_path));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_TYPE", this->_contentType));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_LENGTH", to_string(this->_contentLength)));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("PATH_INFO", this->_path));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
	//_envDatabase.insertData(CgiVariable<std::string, std::string>("QUERY_STRING", request.getQuery()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_METHOD", this->_methodName));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_URI", this->_uri));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PORT", to_string(this->_port)));
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_SOFTWARE", "webserv"));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("REDIRECT_STATUS", "200"));
<<<<<<< HEAD
	// _envDatabase.insertData(CgiVariable<std::string, std::string>("HTTP_HOST", to_string(_serverScope->getRoot() + "/" + _serverScope->getIndex()[0])));
	// for (std::map<std::string, std::string>::iterator it = _query.begin(); it != _query.end(); it++)
	// {
	// 	_envDatabase.insertData(CgiVariable<std::string, std::string>(it->first, it->second));
	// 	std::cout << CYAN << it->first << "=" << it->second << RESET << std::endl;
	// }
=======

/* 	if (_response->getMethodName() == "GET")
	{
		for (std::map<std::string, std::string>::iterator it = _query.begin(); it != _query.end(); it++)
		{
			_envDatabase.insertData(CgiVariable<std::string, std::string>(it->first, it->second));
			std::cout << CYAN << it->first << "=" << it->second << RESET << std::endl;
		}
	} */
>>>>>>> 287e9f52aed247b7830da53bb64a94fb3e140f61
}
