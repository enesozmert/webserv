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
	std::cout << "request->getBody() : " << request->getBody() << std::endl;
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

	keywordFill();

	// std::cout << YELLOW << "CONTENT_LENGTH = " << this->_env["CONTENT_LENGTH"] << RESET << std::endl;
	// std::cout << YELLOW << "SCRIPT_NAME = " << this->_env["SCRIPT_NAME"] << RESET << std::endl;
	// std::cout << YELLOW << "SCRIPT_FILENAME = " << this->_env["SCRIPT_FILENAME"] << RESET << std::endl;
	// std::cout << YELLOW << "CONTENT_TYPE = " << this->_env["CONTENT_TYPE"] << RESET << std::endl;
	// std::cout << YELLOW << "PATH_INFO = " << this->_env["PATH_INFO"] << RESET << std::endl;
	// std::cout << YELLOW << "REQUEST_URI = " << this->_env["REQUEST_URI"] << RESET << std::endl;
	// std::cout << YELLOW << "_body = " << this->_body << RESET << std::endl;
}


std::string		Cgi::executeCgi(const std::string& scriptName) 
{
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
	std::cout << RED << "env0: " << env[0] << RESET << std::endl;
	std::cout << RED << "env1: " << env[1] << RESET << std::endl;

	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	//orijinal stdin ve stdout'u burada tutuyoruz ki kaybetmeyelim.
	//tmpfile() işlevi, geçici bir dosya oluşturur ve dosya tanımlayıcısı için bir FILE işaretçisi döndürür. 
	//Bu işlev, geçici dosyaların oluşturulması ve yönetilmesi için kolay bir yol sağlar.
	//tmpfile() işlevi, oluşturulan dosyanın ismini, konumunu ve boyutunu ayarlar
	// ve dosya kapandığında otomatik olarak silinir.

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	//fileno() işlevi, bir FILE işaretçisine karşılık gelen dosya tanımlayıcısını döndürür.
	int fdIn = fileno(fIn);
	int fdOut = fileno(fOut);

	//if (this->_env["REQUEST_METHOD"] == "POST")
	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);
	//Daha sonra, lseek() fonksiyonu kullanılarak dosya okuma yazma konumu (offset) ayarlanır.
	//Bu durumda, lseek(fdIn, 0, SEEK_SET) ifadesi, dosyanın okuma yazma konumunu dosyanın başına (SEEK_SET) taşır.
	//Bu işlem, sonraki okuma işleminin, dosyanın başlangıcından itibaren yapılmasını sağlar.
	//İkinci parametre, ayarlanacak konumun byte cinsinden belirtilen offsetidir.
	//Üçüncü parametre ise, offsetin nereye göre belirleneceğini belirten bir sabittir. 
	//SEEK_SET, offsetin dosyanın başından itibaren belirlendiğini gösterir.

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
		execve(&scriptName[0], NULL, env);
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
			std::cout << RED << "buffer: " << buffer << RESET << std::endl;
			newBody += buffer;
		}
	}

	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);

	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;

	std::cout << "newBody: " << newBody << std::endl;
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
    _envDatabase.insertData(CgiVariable<std::string, std::string>("SCRIPT_FILENAME", _response->getCgiPass()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_TYPE", _request->getContentType()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("CONTENT_LENGTH", std::to_string(this->_body.length())));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("PATH_INFO", "/Users/eozmert/Desktop/webserv/"));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_METHOD", _response->getMethod()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("REQUEST_URI", "/Users/eozmert/Desktop/webserv/"));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("REMOTEaddr", _serverScope->getHost()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PORT", _serverScope->getPort()));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("SERVER_SOFTWARE", "nginx/webserv"));
    _envDatabase.insertData(CgiVariable<std::string, std::string>("REDIRECT_STATUS", "200"));
}


