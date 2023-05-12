#include "../inc/entity/Request.hpp"

Request::Request()
{
}

Request::~Request()
{
}
Request::Request(const Request& request)
{
    (*this) = request;
}
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
std::string Request::getEnvForCgi()
{
    return (this->_envForCgi);
}
int Request::getPort()
{
    int numberOfPort = std::stoi(this->_port);
    return (numberOfPort);
}
std::vector<std::pair<std::string, float> > Request::getAcceptLanguages()
{
    std::vector<std::pair<std::string, float> > defaults;
    return (defaults);
}
std::string Request::getRaw()
{
    return (this->_raw);
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
int Request::getContentLength()
{
    int numberOfContentLength = std::stoi(this->_contentLength);
    return (numberOfContentLength);
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
void Request::setEnvForCgi(std::string envForCgi)
{
    this->_envForCgi = envForCgi;
}
void Request::setPort(std::string port)
{
    this->_port = port;
}
void Request::setAcceptLanguage(std::string acceptLanguage)
{
    (void)acceptLanguage;
    //none
}
void Request::setRaw(std::string raw)
{
    this->_raw = raw;
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
    _keywordDatabase.insertData(Variable<std::string>("content-type", &this->_contentLength));
    _keywordDatabase.insertData(Variable<std::string>("content-length", &this->_contentLength));
}
std::string Request::getName() const
{
    return (this->name);
}
Request* Request::cloneNew() const
{
    return (new Request());
}
Request* Request::clone() const
{
    return (new Request(*this));
}