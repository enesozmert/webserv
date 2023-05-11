#include "../inc/request/Request.hpp"

Request::Request(): _port(80), return_code(200)
{
	initMethods();
	this->resetHeaders();
	this->_env_for_cgi.clear();
	if (this->return_code != 200)
		std::cerr << "Parse error : " << this->return_code << std::endl;
}

Request::~Request() {}

/*** GETTERS ***/

const std::map<std::string, std::string> &Request::getHeaders() const { return this->_headers; }

const std::map<std::string, std::string> &Request::getEnvforCGI() const { return this->_env_for_cgi; }

const std::string &Request::getMethod() const { return this->_method; }

const std::string &Request::getVersion() const { return this->_version; }

int Request::getRet() const { return this->return_code; }

const std::string &Request::getBody() const { return this->_body; }

int Request::getPort() const { return this->_port; }

const std::string &Request::getPath() const { return this->_path; }

const std::string &Request::getQuery() const { return this->_query; }

const std::string &Request::getRaw() const { return this->_raw; }

const std::vector<std::pair<std::string, float> > &Request::getLanguage() const { return this->_languages; }

/*** SETTERS ***/

void Request::setReturnCode(int return_code) { this->return_code = return_code; }

void Request::setMethod(const std::string &method) { this->_method = method; }

void Request::resetHeaders()
{
	this->_headers.clear();

	this->_headers["Accept-Charsets"] = "";
	this->_headers["Accept-Language"] = "";
	this->_headers["Allow"] = "";
	this->_headers["Auth-Scheme"] = "";
	this->_headers["Authorization"] = "";
	this->_headers["Content-Language"] = "";
	this->_headers["Content-Length"] = "";
	this->_headers["Content-Location"] = "";
	this->_headers["Content-Type"] = "";
	this->_headers["Date"] = "";
	this->_headers["Host"] = "";
	this->_headers["Last-Modified"] = "";
	this->_headers["Location"] = "";
	this->_headers["Referer"] = "";
	this->_headers["Retry-After"] = "";
	this->_headers["Server"] = "";
	this->_headers["Transfer-Encoding"] = "";
	this->_headers["User-Agent"] = "";
	this->_headers["Www-Authenticate"] = "";
	this->_headers["Connection"] = "Keep-Alive";
}

void Request::setLanguage()
{
	// chatgpt
	//  Dil kodlarını ve önceliklerini tutacak bir vektör tanımlayalım.-->_languages
	//  Dil kodlarını ve önceliklerini ayıralım
	std::stringstream ss(this->_headers["Accept-Language"]);
	std::string language;
	while (std::getline(ss, language, ','))
	{
		// Dil önceliğini ve dil kodunu ayıralım
		std::istringstream iss(language);
		std::string code;
		float q = 1.0f; // varsayılan öncelik 1.0'dır
		if (std::getline(iss, code, ';'))
		{
			// Dil kodunu aldık, şimdi dil önceliğini kontrol edelim
			std::string qstr;
			if (std::getline(iss, qstr, '='))
			{
				// Dil önceliği var, bu durumda qstr "q" olmalı
				if (qstr == "q")
				{
					std::getline(iss, qstr);
					q = std::atof(qstr.c_str());
				}
			}
		}
		// Dil kodunu ve önceliğini vektöre ekleyelim
		_languages.push_back(std::make_pair(code, q));
	}
}

void Request::setBody(const std::string &str)
{
	// body kısmını ayarlayıp, sonundaki kaçış karakterlerini kırpıyoruz.Bu kısmı biraz daha araştır. "\r\n\r\n" sonda mı? body ve header arasında mı?
	this->_body.assign(str);
	while (this->_body[this->_body.size() - 1] == '\n' || this->_body[this->_body.size() - 1] == '\r')
	{
		if (this->_body.size() > 0)
			this->_body.resize(this->_body.size() - 1);
	}
	// başında /r/n var mı kontrol etmek gerekiyor mu? str ilk ne olarak geliyor buraya?
}
void Request::initMethods()
{
	this->_methods.push_back("GET");
	this->_methods.push_back("POST");
	this->_methods.push_back("DELETE");
}

std::string Request::createKey(const std::string &line)
{
	std::string key;
	size_t j = 0;
	size_t i = line.find_first_of(':');

	key.append(line, 0, i); // key stringine 0'dan :'a kadar olan kısmı ekliyoruz.

	// std::cout << "key : " << key << std::endl;
	// normalde nasıl geliyor ki büyük küçük harf çevirileri yapıyoruz kontrol et.
	std::transform(key.begin(), key.end(), key.begin(), ::tolower); // bütün stringi ilk küçük harfe çeviriyoruz.
	key[j] = std::toupper(key[j]);									// ilk karakteri büyütüyoruz.
	/// User-Agent--->bunun gibiler için - sonrasını da büyük harfe çeviriyoruz.
	while ((j = key.find_first_of('-', j + 1)) != std::string::npos) // 1. indexten başlayarak - var mı kontrolü yapıyoruz.
	{
		if (j + 1 < key.size())
			key[j + 1] = std::toupper(key[j + 1]);
	}
	return (trim(key, " \n\r\t\f\v"));
}

std::string Request::createValue(const std::string &line)
{
	size_t i;
	std::string value;

	i = line.find_first_of(':');			// User-Agent: Mozilla
	i = line.find_first_not_of(' ', i + 1); // space'ten sonra ilk space olmayan karakter yani 'M'
	if (i != std::string::npos)
		value.append(line, i, std::string::npos); //'M' den başlayıp string sonuna kadar
	return (trim(value, " \n\r\t\f\v"));
}

int Request::checkVersion()
{
	if (this->_version != "HTTP/1.0" && this->_version != "HTTP/1.1")
	{
		this->return_code = 400;
		std::cerr << "BAD HTTP VERSION (" << this->_version << ")" << std::endl;
		return (this->return_code);
	}
	return this->return_code;
}

int Request::checkMethod()
{
	for (size_t i = 0; i < this->_methods.size(); i++)
		if (this->_methods[i] == this->_method)
			return this->return_code;
	std::cerr << "Invalid method requested" << std::endl;
	this->return_code = 400;
	return this->return_code;
}

void Request::parseFirstline(const std::string &str)
{
	std::stringstream s(str);
	s >> this->_method;
	s >> this->_path;
	s >> this->_version;

	std::cout << "method: " << _method << std::endl;
	std::cout << "path: " << _path << std::endl;
	std::cout << "version: " << _version << std::endl;

	this->checkMethod();
	this->checkVersion();
}

// std::string Request::GetnextLine(const std::string &str, size_t &i)
// {
// 	std::string subStr;
// 	size_t j;

// 	if (i == std::string::npos)
// 		return "";
// 	j = str.find_first_of('\n', i);
// 	subStr = str.substr(i, j - i);
// 	if (subStr[subStr.size() - 1] == '\r')
// 	{
// 		if (subStr.size())
// 			subStr.resize(subStr.size() - 1);
// 	}
// 	if (j == std::string::npos)
// 		i = j;
// 	i = j + 1; // i adres olarak geldi o yüzden geldiği yerde de değişecek
// 	// sonunda /r/n olmadan satırı döndürür.
// 	return subStr;
// }

std::string Request::convertHeadertoCGIformat(std::string &key)
{
	std::transform(key.begin(), key.end(), key.begin(), ::toupper);
	for (size_t i = 0; i < key.size(); i++)
	{
		if (key[i] == '-')
			key[i] = '_';
	}
	return "HTTP_" + key;
}

void Request::findQuery()
{
	size_t i;

	i = this->_path.find_first_of('?'); //? 'den sonraki kısmı(query) olarak atarız.
	if (i != std::string::npos)
	{
		this->_query.assign(this->_path, i + 1, std::string::npos);
		this->_path = this->_path.substr(0, i); //?'den önceki kısım da path olur
	}
}

int Request::parse(const std::string &str)
{
	std::cout << "str : " << str << std::endl;
	std::cout << "*********----------------**************" << std::endl;
	std::string key;
	std::string value;
	std::string line;
	std::string fileCleanContents = str;
	std::string delimiter = "\n";
	size_t pos = 0;

	if ((pos = fileCleanContents.find_first_of(delimiter)) != std::string::npos)
    {
        line = fileCleanContents.substr(0, pos + 1);
        fileCleanContents.erase(0, pos + 1);
        std::cout << "line : " << trim(line, "\n\r") << std::endl;
        this->parseFirstline(line);
        line.clear();
    }

	while ((pos = fileCleanContents.find_first_of(delimiter)) != std::string::npos)
	{
		line = fileCleanContents.substr(0, pos + 1);
		fileCleanContents.erase(0, pos + 1);
		// std::cout << "line : " << trim(line, "\n\r") << std::endl;
		key = createKey(line);
		value = createValue(line);
		// key-value'leri _headers mapine ekliyoruz.
		if (this->_headers.count(key))
			this->_headers[key] = value;
		if (key.find("Secret") != std::string::npos) // buna gerek var mı?
			this->_env_for_cgi[convertHeadertoCGIformat(key)] = value;
	}
	if (this->_headers["Www-Authenticate"] != "")
		this->_env_for_cgi["Www-Authenticate"] = this->_headers["Www-Authenticate"];
	this->setLanguage();
	// burayı kontrol et i hangi karakterin üstünde?
	// this->setBody(str.substr(i, std::string::npos));
	this->findQuery();
	return this->return_code;
}

std::ostream &operator<<(std::ostream &os, const Request &re)
{
	std::map<std::string, std::string>::const_iterator it;

	os << "Method : " << re.getMethod() << " |\tHTTP version : ";
	os << re.getVersion() << '\n';
	os << "Port : " << re.getPort() << '\n';
	os << "Path : " << re.getPath() << '\n';

	for (it = re.getHeaders().begin(); it != re.getHeaders().end(); it++)
		os << it->first << ": " << it->second << '\n';

	os << '\n'
	   << "Request body :\n"
	   << re.getBody() << '\n';

	return os;
}