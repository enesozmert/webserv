#include "../inc/cgi/Cgi.hpp"

Cgi::Cgi() {}

Cgi::~Cgi(){}

Cgi::Cgi(const Cgi &cgi)
{
	*this = cgi;
}

Cgi& Cgi::operator=(const Cgi &cgi)
{
	if (this == &cgi)
        return (*this);
    this->_body = cgi._body;
    return (*this);
}

Cgi::Cgi(Request *request, ServerScope* serverScope, Response *response): _request(request), _response(response), _serverScope(serverScope), _body(response->getBody())
{
	this->_query = _response->getQueries();
	keywordFill();
}

std::string     Cgi::executeCgi(std::string scriptName)
{
	int			saveStdin= dup(STDIN_FILENO);
	int			saveStdout = dup(STDOUT_FILENO);
	std::string	newBody;

	char	**env = NULL;
	try {
		env = mapToEnvForm(this->_envDatabase.getAllData());
	}
	catch (const std::bad_alloc& e) {
		std::cerr << e.what() << std::endl;
		return "Status: 500\r\n\r\n";
	}

	int fd[2];
	if (pipe(fd) < 0) {
		std::cerr << RED << "pipe problem" << RESET << std::endl;
	}

	fcntl(fd[0], F_SETFL, O_NONBLOCK);
	fcntl(fd[1], F_SETFL, O_NONBLOCK);


	if (_request->getHttpMethodName().find("POST") != std::string::npos) {
		ssize_t writeResult = write(fd[1], _body.c_str(), _body.length());
		if (writeResult == -1) {
			std::cerr << RED << "write problem: " << strerror(errno) << RESET << std::endl;
		}
		lseek(fd[1], 0, SEEK_SET);
	}

	std::string contentLocation = _response->getContentLocation();
	char *cmd[] =  {&scriptName[0], &contentLocation[0], NULL};

	pid_t pid = fork();
	if (pid == -1)
	{
		std::cerr << "Fork crashed." << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid)
	{
		if (_request->getHttpMethodName().find("POST") != std::string::npos)
		{
            dup2(fd[1], 0);
			close(fd[1]);
		}
		dup2(fd[0], 1);
		close(fd[0]);
		execve(cmd[0], cmd, env);
		std::cerr << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
		exit(1);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(pid, NULL, 0);
		lseek(fd[0], 0, SEEK_SET);

		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fd[0], buffer, CGI_BUFSIZE - 1);
			newBody += buffer;
		}
	}
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	close(saveStdout);
	close(saveStdin);
	close(fd[0]);
	close(fd[1]);

	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;

	return (newBody);
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
    _envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_NAME", _response->getCgiPass()));
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_FILENAME", _response->getCgiPass()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_TYPE", _request->getContentType()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_LENGTH", to_string(_request->getContentLength())));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("PATH_INFO", _response->getContentLocation()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_METHOD", _response->getMethodName()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_URI", _response->getContentLocation()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("REMOTEaddr", _serverScope->getHost()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PORT", _serverScope->getPort()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_SOFTWARE", "nginx/webserv"));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("REDIRECT_STATUS", "200"));
	for (std::map<std::string, std::string>::iterator it = _query.begin(); it != _query.end(); it++)
	{
		_envDatabase.insertData(CgiVariable<std::string, std::string>(it->first, it->second));
		std::cout << CYAN << it->first << "=" << it->second << RESET << std::endl;
	}
}

