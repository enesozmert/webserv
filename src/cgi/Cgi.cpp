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
	this->_env = cgi._env;
    return (*this);
}

Cgi::Cgi(Request *request, ServerScope* server, Response *response): _body(response->getBody())
{
	this->_env.insert(std::make_pair("REDIRECT_STATUS", "200")); //Security needed to execute php-cgi
	this->_env.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
	this->_env.insert(std::make_pair("SCRIPT_FILENAME", response->getPath()));//CGI script'in tam dosya yolunu
	this->_env.insert(std::make_pair("REQUEST_METHOD", response->getMethod()));
	this->_env.insert(std::make_pair("CONTENT_LENGTH", std::to_string(this->_body.length())));
	this->_env.insert(std::make_pair("CONTENT_TYPE", request->getContentType()));
	this->_env.insert(std::make_pair("PATH_INFO", request->getPath()));//CGI script'in ek bilgileri veya parametreleri içeren yol bilgisi(cgi_param)
	this->_env.insert(std::make_pair("REQUEST_URI", request->getPath()));
	this->_env.insert(std::make_pair("REMOTE_ADDR", request->getIp()));
    this->_env.insert(std::make_pair("SERVER_PORT", server->getPort()));
	this->_env.insert(std::make_pair("SERVER_PROTOCOL", request->getVersion()));
	this->_env.insert(std::make_pair("SERVER_SOFTWARE", response->getServerName()));

	std::cout << YELLOW << "CONTENT_LENGTH = " << this->_env["CONTENT_LENGTH"] << RESET << std::endl;
	std::cout << YELLOW << "SCRIPT_NAME = " << this->_env["SCRIPT_NAME"] << RESET << std::endl;
	std::cout << YELLOW << "SCRIPT_FILENAME = " << this->_env["SCRIPT_FILENAME"] << RESET << std::endl;
	std::cout << YELLOW << "CONTENT_TYPE = " << this->_env["CONTENT_TYPE"] << RESET << std::endl;
	std::cout << YELLOW << "PATH_INFO = " << this->_env["PATH_INFO"] << RESET << std::endl;
	std::cout << YELLOW << "REQUEST_URI = " << this->_env["REQUEST_URI"] << RESET << std::endl;
	std::cout << YELLOW << "_body = " << this->_body << RESET << std::endl;
}


std::string		Cgi::executeCgi(const std::string& scriptName) 
{
	std::cout << YELLOW << "cgi scriptName : " << scriptName << RESET << std::endl;

	int			saveStdin= dup(STDIN_FILENO);;
	int			saveStdout = dup(STDOUT_FILENO);;
	std::string	newBody;

	char	**env = nullptr;
	try {
		env = mapToEnvForm(this->_env);//envleri map'ten char **str'ye çeviriyoruz.
	}
	catch (const std::bad_alloc& e) {
		std::cerr << e.what() << std::endl;
		return "Status: 500\r\n\r\n";
	}

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
		execve(scriptName.c_str(), nullptr, env);
		std::cerr << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
		exit(1);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(-1, nullptr, 0);
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


