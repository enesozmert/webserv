#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "../database/DataBase.hpp"
#include "IRequest.hpp"
#include "Variable.hpp"
#include "HttpMethod.hpp"

class HttpMethod;
class Request : public IRequest
{
    private:
    	std::string _version;
    	std::string _path;
    	std::string _body;
    	std::string	_query;
        std::string _envForCgi;
        std::string	_acceptLanguages;
        std::string _host;
        std::string _ip;
    	std::string _port;
        std::string _userAgent;
        std::string _accept;
        std::string _acceptEncoding;
        std::string _acceptCharset;
        std::string _keepAlive;
        std::string _connection;
        std::string _referer;
        std::string _contentType;
        std::string _contentLength;
        std::string _httpMethodName;
        std::string _authScheme;
        std::string _authorization;
        int         return_code;

        HttpMethod  _httpMethod;

        DataBase<Variable<std::string> > _keywordDatabase;
    public:
        Request();
        ~Request();
        Request(const Request& request);

        std::string getVersion();
        std::string getPath();
        std::string getBody();
        std::string getQuery();
        std::string getEnvForCgi();
        int         getPort();
        std::string getIp();
        std::vector<std::pair<std::string, float> > getAcceptLanguages();
        std::string getHost();
        std::string getUserAgent();
        std::string getAccept();
        std::string getAcceptEncoding();
        std::string getAcceptCharset();
        std::string getKeepAlive();
        std::string getConnection();
        std::string getReferer();
        std::string getContentType();
        int         getContentLength();
        std::string getHttpMethodName();
        std::string getAuthScheme();
        std::string getAuthorization();
        DataBase<Variable<std::string> > getKeywordDataBase();

        void setVersion(std::string version);
        void setPath(std::string path);
        void setBody(std::string body);
        void setQuery(std::string query);
        void setEnvForCgi(std::string envForCgi);
        void setPort(std::string port);
        void setAcceptLanguage(std::string acceptLanguage);
        void setHost(std::string host);
        void setIp(std::string ip);
        void setUserAgent(std::string userAgent);
        void setAccept(std::string accept);
        void setAcceptEncoding(std::string acceptEncoding);
        void setAcceptCharset(std::string acceptCharset);
        void setKeepAlive(std::string keepAlive);
        void setConnection(std::string connection);
        void setReferer(std::string referer);
        void setContentType(std::string contentType);
        void setContentLength(std::string contentLength);
        void setHttpMethodName(std::string httpMethodName);
        void setAuthScheme(std::string authScheme);
        void setAuthorization(std::string authorization);
        void setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);

        void keywordFill();

        std::string getName() const;
        Request* clone() const;
        Request* cloneNew() const;
};