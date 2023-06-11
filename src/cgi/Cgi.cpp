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
	//std::cout << "*******old_body******\n " << _body << std::endl;
	// this->_env.insert(std::make_pair("REDIRECT_STATUS", "200")); //Security needed to execute php-cgi
	// this->_env.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
	// this->_env.insert(std::make_pair("SCRIPT_FILENAME", response->getCgiPass()));//CGI script'in tam dosya yolunu
	// this->_env.insert(std::make_pair("REQUEST_METHOD", response->getMethod()));
	// this->_env.insert(std::make_pair("CONTENT_LENGTH", std::to_string(this->_body.length())));
	// this->_env.insert(std::make_pair("CONTENT_TYPE", request->getContentType()));
	// this->_env.insert(std::make_pair("PATH_INFO", request->getPath()));//CGI script'in ek bilgileri veya parametreleri içeren yol bilgisi(cgi_param)
	// this->_env.insert(std::make_pair("REQUEST_URI", request->getPath()));
	// this->_env.insert(std::make_pair("REMOTEaddr", serverScope->getHost()));
    // this->_env.insert(std::make_pair("SERVER_PORT", serverScope->getPort()));
	// this->_env.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
	// this->_env.insert(std::make_pair("SERVER_SOFTWARE", "nginx/webserv"));


	// this->_request = request;
	// this->_serverScope = serverScope;
	// this->_response = response;
	this->_query = _response->getQueries();

	keywordFill();

	// std::cout << YELLOW << "CONTENT_LENGTH = " << this->_env["CONTENT_LENGTH"] << RESET << std::endl;
	// std::cout << YELLOW << "SCRIPT_NAME = " << this->_env["SCRIPT_NAME"] << RESET << std::endl;
	// std::cout << YELLOW << "SCRIPT_FILENAME = " << this->_env["SCRIPT_FILENAME"] << RESET << std::endl;
	// std::cout << YELLOW << "CONTENT_TYPE = " << this->_env["CONTENT_TYPE"] << RESET << std::endl;
	// std::cout << YELLOW << "PATH_INFO = " << this->_env["PATH_INFO"] << RESET << std::endl;
	// std::cout << YELLOW << "REQUEST_URI = " << this->_env["REQUEST_URI"] << RESET << std::endl;
	// std::cout << YELLOW << "_body = " << this->_body << RESET << std::endl;
}


std::string     Cgi::executeCgi(std::string scriptName)
{
	// int pip[2];
    // int ret = 0;
    // pid_t child = 0;
    // pid_t parent = 0;
	// int checke_prob = 0;
    // int fd = open("tmp", O_CREAT | O_TRUNC | O_WRONLY | O_NONBLOCK, 0777);

    // int checke_Cwrite = write(fd, _body.c_str(), _body.size());
	// if (checke_Cwrite == 0)
	// {
	// 	checke_prob = 0;
	// }
	// else if (checke_Cwrite == -1)
	// {
	// 	checke_prob = -1;
	// }
	// close(fd);


	// char	**env = NULL;
	// try {
	// 	env = mapToEnvForm(this->_envDatabase.getAllData());//envleri map'ten char **str'ye çeviriyoruz.
	// }
	// catch (const std::bad_alloc& e) {
	// 	std::cerr << e.what() << std::endl;
	// 	return "Status: 500\r\n\r\n";
	// }
	//  for (int i = 0; env[i] != nullptr; ++i) {
    //     std::cout << RED << env[i] << RESET << std::endl;
    // }

	// char *echo[3] = {(char *)"cat", (char *)"tmp", NULL};

	// std::string contentLocation = _response->getContentLocation();
	// std::cout << "contentLocation" << contentLocation << std::endl;
    // char *cmd[] =  {&scriptName[0], &contentLocation[0], NULL};
    // if (pipe(pip) == -1)
    // {
    //     perror("CGI part : Pipe failed");
    //     exit(1);
    // }
    // child = fork();
    // int tmp = open(".tmp", O_CREAT | O_TRUNC | O_NONBLOCK | O_RDWR, 0777);
	// if (child == -1)
    // {
    //     std::cerr << "Fork failed" << std::endl;
	// 	return ("Status: 500\r\n\r\n");
    // }
    // else if (!child)
    // {
    //     dup2(pip[1], 1);
    //     close(pip[0]);
    //     ret = execvp(echo[0], echo);
    // }
    // else
    // {
    //     int status2;

    //     wait(&status2);
    //     parent = fork();
    //     if (!parent)
    //     {
    //         dup2(pip[0], 0);
    //         dup2(tmp, 1);
    //         close(pip[1]);
    //         ret = execve(cmd[0], cmd, env);
    //     }
    //     else
    //     {
    //         int status;

    //         wait(&status);
    //         close(pip[0]);
    //         close(pip[1]);
    //         close(tmp);

    //         usleep(100000);

    //         tmp = open(".tmp", O_NONBLOCK | O_RDONLY);

    //         char buf[65535];
    //         bzero(buf, 65535);
    //         int checkRead = read(tmp, buf, 65535);
	// 		if (checkRead == 0)
	// 			checke_prob = 0;
	// 		else if (checkRead == -1)
	// 			checke_prob = -1;
    //         close(tmp);
    //         _body = std::string(buf);
    //         remove("tmp");
    //         remove(".tmp");

    //         delete[] env;
    //         return (_body);
    //     }
    // }

    // delete[] env;
    // return _body;

	std::cout << YELLOW << "cgi scriptName : " << scriptName << RESET << std::endl;

	int			saveStdin= dup(STDIN_FILENO);;
	int			saveStdout = dup(STDOUT_FILENO);;
	std::string	newBody;

	char	**env = NULL;
	try {
		env = mapToEnvForm(this->_envDatabase.getAllData());//envleri map'ten char **str'ye çeviriyoruz.
	}
	catch (const std::bad_alloc& e) {
		std::cerr << e.what() << std::endl;
		return "Status: 500\r\n\r\n";
	}

	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	// orijinal stdin ve stdout'u burada tutuyoruz ki kaybetmeyelim.
	// tmpfile() işlevi, geçici bir dosya oluşturur ve dosya tanımlayıcısı için bir FILE işaretçisi döndürür. 
	// Bu işlev, geçici dosyaların oluşturulması ve yönetilmesi için kolay bir yol sağlar.
	// tmpfile() işlevi, oluşturulan dosyanın ismini, konumunu ve boyutunu ayarlar
	// ve dosya kapandığında otomatik olarak silinir.

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	// fileno() işlevi, bir FILE işaretçisine karşılık gelen dosya tanımlayıcısını döndürür.
	int fdIn = fileno(fIn);
	int fdOut = fileno(fOut);
	fcntl(fdIn, F_SETFL, O_NONBLOCK);
	fcntl(fdOut, F_SETFL, O_NONBLOCK);
	

	// if (this->_env["REQUEST_METHOD"] == "POST")
	if (write(fdIn, _body.c_str(), _body.size()) == -1)
		std::cerr << RED << "write problem" << RESET << std::endl;
	lseek(fdIn, 0, SEEK_SET);
	// Daha sonra, lseek() fonksiyonu kullanılarak dosya okuma yazma konumu (offset) ayarlanır.
	// Bu durumda, lseek(fdIn, 0, SEEK_SET) ifadesi, dosyanın okuma yazma konumunu dosyanın başına (SEEK_SET) taşır.
	// Bu işlem, sonraki okuma işleminin, dosyanın başlangıcından itibaren yapılmasını sağlar.
	// İkinci parametre, ayarlanacak konumun byte cinsinden belirtilen offsetidir.
	// Üçüncü parametre ise, offsetin nereye göre belirleneceğini belirten bir sabittir. 
	// SEEK_SET, offsetin dosyanın başından itibaren belirlendiğini gösterir.

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
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		close(fdIn);
		close(fdOut);
		execve(cmd[0], cmd, env);
		std::cerr << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
		exit(1);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fdOut, buffer, CGI_BUFSIZE - 1);
			newBody += buffer;
		}
	}

	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	close(saveStdout);
	close(saveStdin);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);

	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;

	std::cout << "********newBody*******\n" << newBody << std::endl;
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

