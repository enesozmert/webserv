#include "Cgi.hpp"

Cgi::Cgi(Request &request, Config &config): _body(request.getBody())
{
	//request headerının olduğu map'i çektik
	std::map<std::string, std::string>	headers = request.getHeaders();

	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		this->_env.insert(std::make_pair("AUTH_TYPE", headers["Authorization"]));

	this->_env.insert(std::make_pair("REDIRECT_STATUS", "200")); //Security needed to execute php-cgi
	this->_env.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
	this->_env.insert(std::make_pair("SCRIPT_NAME",config.getPath()));
	this->_env.insert(std::make_pair("SCRIPT_FILENAME", config.getPath()));
	this->_env.insert(std::make_pair("REQUEST_METHOD", request.getMethod()));
	this->_env.insert(std::make_pair("CONTENT_LENGTH", to_string(this->_body.length())));
	this->_env.insert(std::make_pair("CONTENT_TYPE", headers["Content-Type"]));
	this->_env.insert(std::make_pair("PATH_INFO", request.getPath())); //might need some change, using config path/contentLocation
	this->_env.insert(std::make_pair("PATH_TRANSLATED", request.getPath())); //might need some change, using config path/contentLocation
	this->_env.insert(std::make_pair("QUERY_STRING", request.getQuery()));
	this->_env.insert(std::make_pair("REMOTEaddr", to_string(config.getHostPort().host)));
	this->_env.insert(std::make_pair("REMOTE_IDENT", headers["Authorization"]));
	this->_env.insert(std::make_pair("REMOTE_USER", headers["Authorization"]));
	this->_env.insert(std::make_pair("REQUEST_URI", request.getPath() + request.getQuery()));
	
	if (headers.find("Hostname") != headers.end())
		this->_env.insert(std::make_pair("SERVER_NAME", headers["Hostname"]));
	else
		this->_env.insert(std::make_pair("SERVER_NAME", this->_env["REMOTEaddr"]));

    this->_env.insert(std::make_pair("SERVER_PORT", to_string(config.getHostPort().port)));
	this->_env.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
	//this->_env.insert(std::make_pair("SERVER_SOFTWARE", "php-cgi/1.1"));
	this->_env.insert(std::make_pair("SERVER_SOFTWARE", "Weebserv/1.0"));//burayı değiştir
	//config dosyasındaki cgi_param'ların tutulduğu map'i ekliyoruz. Birden fazla olabildiği için map içinde
	this->_env.insert(config.getCgiParam().begin(), config.getCgiParam().end());

}


std::string		Cgi::executeCgi(const std::string& scriptName) 
{
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	char		**env;
	std::string	newBody;

	try {
		env = maptoStr(this->_env);//envleri map'ten char **str'ye çeviriyoruz.
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


