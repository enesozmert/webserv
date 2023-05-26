#include "../inc/cgi/Cgi.hpp"

Cgi::Cgi(Request *request, ServerScope* server, std::string& _path, std::map<std::string, std::string> envParams): _body(request->getBody())
{
	this->_env.insert(std::make_pair("REDIRECT_STATUS", "200")); //Security needed to execute php-cgi
	this->_env.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
	this->_env.insert(std::make_pair("SCRIPT_NAME", _path));
	this->_env.insert(std::make_pair("SCRIPT_FILENAME", _path));
	this->_env.insert(std::make_pair("REQUEST_METHOD", request->getHttpMethodName()));
	this->_env.insert(std::make_pair("CONTENT_LENGTH", std::to_string(this->_body.length())));
	this->_env.insert(std::make_pair("CONTENT_TYPE", request->getContentType()));
	this->_env.insert(std::make_pair("PATH_INFO", request->getPath())); //might need some change, using config path/contentLocation
	this->_env.insert(std::make_pair("PATH_TRANSLATED", request->getPath())); //might need some change, using config path/contentLocation
	this->_env.insert(std::make_pair("QUERY_STRING", request->getQuery()));
	this->_env.insert(std::make_pair("REMOTEaddr", server->getHost()));
	this->_env.insert(std::make_pair("REQUEST_URI", request->getPath() + request->getQuery()));
    this->_env.insert(std::make_pair("SERVER_PORT", server->getPort()));
	this->_env.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
	this->_env.insert(std::make_pair("SERVER_SOFTWARE", "php-cgi/1.1"));
	this->_env.insert(std::make_pair("SERVER_SOFTWARE", "webserv"));//burayı değiştirebiliriz.
	this->_env.insert(envParams.begin(), envParams.end());//config dosyasındaki cgi_param'ların tutulduğu map'i ekliyoruz.
}


std::string		Cgi::executeCgi(const std::string& scriptName) 
{
	std::cout << RED << "Cgi çalışmaya başladı" << RESET << std::endl;
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	char		**env;
	std::string	newBody;

	try {
		env = mapToEnvForm(this->_env);//envleri map'ten char **str'ye çeviriyoruz.
	}
	catch (std::bad_alloc &e) {
		std::cerr << e.what() << std::endl;
	}

	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	//orijinal stdin ve stdout'u burada tutuyoruz ki kaybetmeyelim.
	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);
	//tmpfile() işlevi, geçici bir dosya oluşturur ve dosya tanımlayıcısı için bir FILE işaretçisi döndürür. 
	//Bu işlev, geçici dosyaların oluşturulması ve yönetilmesi için kolay bir yol sağlar.
	//tmpfile() işlevi, oluşturulan dosyanın ismini, konumunu ve boyutunu ayarlar
	// ve dosya kapandığında otomatik olarak silinir.

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	//fileno() işlevi, bir FILE işaretçisine karşılık gelen dosya tanımlayıcısını döndürür.
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);
	//Daha sonra, lseek() fonksiyonu kullanılarak dosya okuma yazma konumu (offset) ayarlanır.
	//Bu durumda, lseek(fdIn, 0, SEEK_SET) ifadesi, dosyanın okuma yazma konumunu dosyanın başına (SEEK_SET) taşır.
	//Bu işlem, sonraki okuma işleminin, dosyanın başlangıcından itibaren yapılmasını sağlar.
	//İkinci parametre, ayarlanacak konumun byte cinsinden belirtilen offsetidir.
	//Üçüncü parametre ise, offsetin nereye göre belirleneceğini belirten bir sabittir. 
	//SEEK_SET, offsetin dosyanın başından itibaren belirlendiğini gösterir.

	pid = fork();

	if (pid == -1)
	{
		std::cerr << "Fork crashed." << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid)
	{
		char * const * nll = NULL;

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(scriptName.c_str(), nll, env);
		std::cerr << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
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

	if (!pid)
		exit(0);

	return (newBody);
}

Cgi::~Cgi(){}
