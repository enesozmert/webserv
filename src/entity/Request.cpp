#include "../inc/entity/Request.hpp"

Request::Request()
{
    this->_returnCode = 200;
    this->keywordFill();
    this->_body = "";
}
Request::~Request() {}

std::string Request::getVersion()
{
    return (this->_version);
}
std::string Request::getPath()
{
    return (this->_path);
}
std::string Request::getBody()
{
    return (this->_body);
}
std::string Request::getQuery()
{
    return (this->_query);
}
int Request::getMulti()
{
    return (this->_multi);
}
int Request::getPort()
{
    std::stringstream ss(this->_host);
    std::string ip;
    std::string portStr;
    int numberOfPort;

    if (this->_host.find(":") == std::string::npos)
        return (80);
    std::getline(ss, ip, ':');
    std::getline(ss, portStr);

    std::istringstream portStream(portStr);
    portStream >> numberOfPort;
    return (numberOfPort);
}
std::string Request::getIp()
{
    std::stringstream ss(this->_host);
    std::string ip;

    if (this->_host.find(":") == std::string::npos)
        return ("");
    std::getline(ss, ip, ':');
    return (ip);
}

std::vector<std::pair<std::string, float> > Request::getAcceptLanguages()
{
    std::vector<std::pair<std::string, float> > _languages;
    std::stringstream ss(this->_acceptLanguages);
	std::string language;
	while (std::getline(ss, language, ','))
	{
		std::istringstream iss(language);
		std::string code;
		float q = 1.0f;
		if (std::getline(iss >> std::ws, code, ';'))
		{
			std::string qstr;
			if (std::getline(iss >> std::ws, qstr, '='))
			{
				if (qstr == "q")
				{
					std::getline(iss >> std::ws, qstr);
					q = std::atof(qstr.c_str());
				}
			}
		}
		_languages.push_back(std::make_pair(code, q));
	}
    return (_languages);
}
std::string Request::getHost()
{
    return (this->_host);
}
std::string Request::getUserAgent()
{
    return (this->_userAgent);
}
std::string Request::getAccept()
{
    return (this->_accept);
}
std::string Request::getAcceptEncoding()
{
    return (this->_acceptEncoding);
}
std::string Request::getAcceptCharset()
{
    return (this->_acceptCharset);
}
std::string Request::getKeepAlive()
{
    return (this->_keepAlive);
}
std::string Request::getConnection()
{
    return (this->_connection);
}
std::string Request::getReferer()
{
    return (this->_referer);
}
std::string Request::getContentType()
{
    return (this->_contentType);
}
size_t Request::getContentLength()
{
    return (atoi(this->_contentLength.c_str()));
}

std::string Request::getHttpMethodName()
{
    bool result = this->_httpMethod.checkMethodName(this->_httpMethodName);
    return (result == true ? this->_httpMethodName : "");
}

int Request::getReturnCode()
{
    return (_returnCode);
}
std::string Request::getAuthScheme()
{
    return (this->_authScheme);
}

std::string Request::getAuthorization()
{
    return (this->_authorization);
}
DataBase<Variable<std::string> > Request::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}
void Request::setVersion(std::string version)
{
    this->_version = version;
}
void Request::setPath(std::string path)
{
    this->_path = path;
}
void Request::setBody(std::string body)
{
    this->_body = body;
}
void Request::setQuery(std::string query)
{
    this->_query = query;
}
void Request::setPort(std::string port)
{
    this->_port = port;
}
void Request::setIp(std::string ip)
{
    this->_ip = ip;
}
void Request::setAcceptLanguage(std::string acceptLanguage)
{
    (void)acceptLanguage;
    //none
}
void Request::setHost(std::string host)
{
    this->_host = host;
}
void Request::setAccept(std::string accept)
{
    this->_accept = accept;
}
void Request::setAcceptEncoding(std::string acceptEncoding)
{
    this->_acceptEncoding = acceptEncoding;
}
void Request::setAcceptCharset(std::string acceptCharset)
{
    this->_acceptCharset = acceptCharset;
}
void Request::setKeepAlive(std::string keepAlive)
{
    this->_keepAlive = keepAlive;
}
void Request::setConnection(std::string connection)
{
    this->_connection = connection;
}
void Request::setReferer(std::string referer)
{
    this->_referer = referer;
}
void Request::setContentType(std::string contentType)
{
    this->_contentType = contentType;
}
void    Request::setContentLenght(std::string contentLength)
{
    this->_contentLength = contentLength;
}
void Request::setHttpMethodName(std::string httpMethodName)
{
    bool result = _httpMethod.checkMethodName(httpMethodName);
    if (result == true)
        this->_httpMethodName = httpMethodName;
}
void Request::setAuthScheme(std::string authScheme)
{
    this->_authScheme = authScheme;
}
void Request::setAuthorization(std::string authorization)
{
    this->_authorization = authorization;
}
void Request::setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void Request::keywordFill()
{
    _keywordDatabase.insertData(Variable<std::string>("host", &this->_host));
    _keywordDatabase.insertData(Variable<std::string>("user-agent", &this->_userAgent));
    _keywordDatabase.insertData(Variable<std::string>("accept", &this->_accept));
    _keywordDatabase.insertData(Variable<std::string>("accept-language", &this->_acceptLanguages));
    _keywordDatabase.insertData(Variable<std::string>("accept-encoding", &this->_acceptEncoding));
    _keywordDatabase.insertData(Variable<std::string>("accept-charset", &this->_acceptCharset));
    _keywordDatabase.insertData(Variable<std::string>("keep-alive", &this->_keepAlive));
    _keywordDatabase.insertData(Variable<std::string>("connection", &this->_connection));
    _keywordDatabase.insertData(Variable<std::string>("referer", &this->_referer));
    _keywordDatabase.insertData(Variable<std::string>("content-type", &this->_contentType));
    _keywordDatabase.insertData(Variable<std::string>("content-length", &this->_contentLength));
    _keywordDatabase.insertData(Variable<std::string>("auth-scheme", &this->_authScheme));
    _keywordDatabase.insertData(Variable<std::string>("authorization", &this->_authorization));
}
Request* Request::cloneNew() const
{
    return (new Request());
}
Request* Request::clone() const
{
    return (new Request(*this));
}

std::string Request::getName() const
{
    return (this->name);
}