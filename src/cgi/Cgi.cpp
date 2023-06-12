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
	int saveStdin = dup(STDIN_FILENO);
	int saveStdout = dup(STDOUT_FILENO);
	std::cout << PURPLE << "CGI" << RESET << std::endl;
	std::cout << PURPLE << "oldBody" << RESET << "\n" << _body << std::endl;
	int request_body_pipe[2];
	int cgi_result_pipe[2];
	std::string	newBody;

	char	**env = NULL;
	try {
		env = mapToEnvForm(this->_envDatabase.getAllData());
	}
	catch (const std::bad_alloc& e) {
		std::cerr << e.what() << std::endl;
		return "Status: 500\r\n\r\n";
	}

	if (pipe(request_body_pipe) < 0) {
		std::cerr << RED << "pipe problem" << RESET << std::endl;
	}
	fcntl(request_body_pipe[1], F_SETFL, O_NONBLOCK);

	if (pipe(cgi_result_pipe) < 0) {
		std::cerr << RED << "pipe problem" << RESET << std::endl;
	}
	fcntl(cgi_result_pipe[0], F_SETFL, O_NONBLOCK);


	if (_request->getHttpMethodName().find("POST") != std::string::npos) {
		ssize_t writeResult = write(request_body_pipe[1], _body.c_str(), _body.length());
		if (writeResult == -1) {
			std::cerr << RED << "write problem: " << strerror(errno) << RESET << std::endl;
		}
		fcntl(request_body_pipe[0], F_SETFL, O_NONBLOCK);
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
        dup2(cgi_result_pipe[1], STDOUT_FILENO);
		dup2(request_body_pipe[0], STDIN_FILENO);
		close(request_body_pipe[1]);
		close(cgi_result_pipe[0]);
		close(request_body_pipe[0]);
		close(cgi_result_pipe[1]);
		execve(cmd[0], cmd, env);
		std::cerr << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
		exit(1);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(pid, NULL, 0);

		lseek(cgi_result_pipe[0], 0, SEEK_SET);
		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(cgi_result_pipe[0], buffer, CGI_BUFSIZE - 1);
			newBody += buffer;
		}
		close(request_body_pipe[0]);
		close(cgi_result_pipe[1]);
		close(request_body_pipe[1]);
		close(cgi_result_pipe[0]);
		dup2(saveStdin, STDIN_FILENO);
		dup2(saveStdout, STDOUT_FILENO);
		close(saveStdout);
		close(saveStdin);
	}

	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;

	std::cout << PURPLE << "newBody" << RESET << "\n" << newBody << std::endl;
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
	_envDatabase.insertData(CgiVariable<std::string, std::string>("UPLOAD_PATH", "/Website_to_test/uploads/"));
	for (std::map<std::string, std::string>::iterator it = _query.begin(); it != _query.end(); it++)
	{
		_envDatabase.insertData(CgiVariable<std::string, std::string>(it->first, it->second));
		std::cout << CYAN << it->first << "=" << it->second << RESET << std::endl;
	}
}

