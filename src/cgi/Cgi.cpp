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

Cgi::Cgi(Request *request, ServerScope *serverScope, Response *response) : _request(request), _response(response), _serverScope(serverScope), _body(response->getBody())
{
	this->_query = _response->getQueries();
	keywordFill();

	try
	{
		env = mapToEnvForm(this->_envDatabase.getAllData());
	}
	catch (const std::bad_alloc &e)
	{
		std::cerr << e.what() << std::endl;
		// return "Status: 500\r\n\r\n";
	}
}

/* std::string     Cgi::executeCgi(std::string scriptName)
{
	std::string newBody;
	int		readed;
	char	output[4096];
	int pipeFd[2];
	int pipeo[2];
	char **env = NULL;

	try {
		env = mapToEnvForm(this->_envDatabase.getAllData());
	}
	catch (const std::bad_alloc &e) {
		std::cerr << e.what() << std::endl;
		return "Status: 500\r\n\r\n";
	}

	if (pipe(pipeFd) == -1)
		return "Status: 500\r\n\r\n";
	if (pipe(pipeo) == -1)
		return "Status: 500\r\n\r\n";

	if (_request->getHttpMethodName().find("POST") != std::string::npos) {
		if (write(pipeFd[1], _body.c_str(), _body.size()) == -1) {
			std::cerr << RED << "Write problem" << RESET << std::endl;
		}
	}

	close(pipeFd[1]);
	std::cout << "scriptName : " << scriptName << std::cout;
	std::string contentLocation = _response->getContentLocation();
	char *cmd[] =  {&scriptName[0], &contentLocation[0], NULL};
	if (!fork())
	{

		close(pipeo[0]);
		dup2(pipeo[1], 1);
		close(pipeo[1]);

		if (_request->getHttpMethodName().find("POST") != std::string::npos)
			dup2(pipeFd[0], 0);
		close(pipeFd[0]);

		execve(cmd[0], cmd, env);
		std::cout << "Execv Err!" << std::endl << std::flush;
		exit(-1);
	}
	//wait(NULL);
	close(pipeFd[0]);
	close(pipeo[1]);

	readed = read(pipeo[0], output, 4096);
	if (readed == 0)
		std::cout << "Cgi Read Fail!" << std::endl << std::flush;
	close(pipeo[0]);
	output[readed] = 0;
	std::cout << "std::string(output, readed) : " << std::string(output, readed) << std::endl;
	return (std::string(output, readed));
} */

std::string Cgi::executeCgi(std::string scriptName)
{
	char buffer[CGI_BUFSIZE] = {0};
	int ret = 1;
	int ret_sum = 0;

	std::cout << PURPLE << "CGI" << RESET << std::endl;
	std::cout << PURPLE << "oldBody" << RESET << "\n"
			  << _body << std::endl;

	if (pipe(request_body_pipe) < 0)
		std::cerr << RED << "pipe problem" << RESET << std::endl;

	if (pipe(cgi_result_pipe) < 0)
		std::cerr << RED << "pipe problem" << RESET << std::endl;

	if (_request->getHttpMethodName().find("POST") != std::string::npos)
	{
		ssize_t writeResult = write(request_body_pipe[1], _body.c_str(), _body.length());
		if (writeResult == -1)
		{
			std::cerr << RED << "write problem: " << strerror(errno) << RESET << std::endl;
		}
	}
	close(request_body_pipe[1]);

	std::string contentLocation = _response->getContentLocation();
	char *cmd[] = {&scriptName[0], &contentLocation[0], NULL};
	std::cout << PURPLE << "contentLocation[0] " << RESET << &contentLocation[0] << std::endl;
	std::cout << PURPLE << "&scriptName[0] " << RESET << &scriptName[0] << std::endl;

	pid_t pid = fork();
	if (pid == -1)
	{
		std::cerr << "Fork crashed." << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (pid == 0)
	{
		close(cgi_result_pipe[0]);
		dup2(cgi_result_pipe[1], STDOUT_FILENO);
		close(cgi_result_pipe[1]);
		if (_request->getHttpMethodName().find("POST") != std::string::npos)
		{
			dup2(request_body_pipe[0], STDIN_FILENO);
		}
		close(request_body_pipe[0]);

		execve(cmd[0], cmd, env);
		std::cerr << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
		exit(1);
	}
	else
	{
		if (waitpid(-1, NULL, 0) == -1)
		{
			exit(1);
			return "Status: 500\r\n\r\n";
		}

		close(request_body_pipe[0]);
		close(cgi_result_pipe[1]);
		
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(cgi_result_pipe[0], buffer, CGI_BUFSIZE - 1);
			ret_sum += ret;
			newBody += std::string(buffer);
		}
		close(cgi_result_pipe[0]);

		for (size_t i = 0; env[i]; i++)
			delete[] env[i];
		delete[] env;
	}

	std::cout << PURPLE << "newBody" << RESET << "\n" << newBody << std::endl;
	return (std::string(newBody, ret_sum));
}

void Cgi::upload()
{
	std::string upload_path = "/Website_to_test/uploads/" + _query["filename"];
	int fd;
	fd = open(upload_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
	int check = write(fd, newBody.c_str(), newBody.length());
	if (check == -1)
		std::cerr << RED << "write problem: " << strerror(errno) << RESET << std::endl;
	close(fd);
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
	_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_NAME", _response->getContentLocation()));
	// std::string res = "Website_to_test/";
	// std::string res1 = "upload.php";
	// std::string abc = res + res1;
	//_envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_FILENAME", abc));
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
	//_envDatabase.insertData(CgiVariable<std::string, std::string>("HTTP_HOST", "200"));
	for (std::map<std::string, std::string>::iterator it = _query.begin(); it != _query.end(); it++)
	{
		_envDatabase.insertData(CgiVariable<std::string, std::string>(it->first, it->second));
		std::cout << CYAN << it->first << "=" << it->second << RESET << std::endl;
	}
}
