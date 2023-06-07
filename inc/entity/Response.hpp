#pragma once

#include <list>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include "../entity/HttpScope.hpp"
#include "../http/HttpStatusCode.hpp"
#include "../http/HttpContentType.hpp"
#include "../entity/Request.hpp"
#include "../utils/Utils.hpp"
#include "../cgi/Cgi.hpp"
#include "../database/DataBase.hpp"

class Response
{
private:
    std::string                         _response;
    int                                 statusCode;
    std::string					        _type;
    std::map<int, std::string>	        staticErrorPage;
    std::map<int, std::string>	        _errors;
    std::vector<std::string>	        _indexs;
    std::string					        _path;
    std::string					        _index;
    std::string                         _locationRootPath;
    std::string                         _serverRootPath;
	std::string					        _cgi_pass;
    std::string                         cgiType;
    std::string					        _error_page;
    std::map<std::string, std::string>	_queries;
    std::string					        _method;
    std::string					        _allows;
    std::vector<std::string>		    _allow_methods;
	std::string					        _contentLanguage;
	std::string					        _contentLength;
	std::string					        _contentLocation;
	std::string					        _contentType;
	std::string					        _date;
	std::string					        _lastModified;
	std::string					        _server;
    std::string                         _host;
    int                                 _port;
    int                                 _clientBodybufferSize;
    std::string					        _body;
	std::string					        _location;
	std::string					        _retryAfter;
	std::string					        _transferEncoding;
	std::string                         _wwwAuthenticate;
    HttpContentType                     _httpContentType;
    HttpStatusCode                      _httpStatusCode;
    DataBase<Variable<std::string> >    _keywordDatabase;
    bool                                _isAutoIndex;
    std::string                         _raw;
public:
    Response();
    ~Response();
    Response(const Response &response);
    Response &operator=(const Response &response);

    int                                 getStatusCode();
    std::string                         getHeader();
    std::string                         getPath();
    std::string                         getBody();
    std::string                         getMethod();
    std::string                         getCgiPass();
    std::map<std::string, std::string>  getQueries();
    std::string                         getResponse();
    DataBase<Variable<std::string> >    getKeywordDataBase();
    std::string                         getServerName();
    std::string                         getContentLocation();
    std::string                         getRaw();

    void                        setAllowMethods(std::vector<std::string> methods);
    void	                    setContentType();
    void                        setQueries();
    void	                    setDate();
    void	                    setLastModified();
    void                        setAutoIndex(std::string _autoIndex);
    void	                    setIndexs(std::vector<std::string> _locationIndex, std::vector<std::string> _serverIndex);
    void                        setStaticErrorPage();
    int                         setPaths(ServerScope *server, LocationScope *location, std::string path);
    void                        setClientBodyBufferSize(std::string bodyBufferSize);
    int                         setResponse(Request *request, ServerScope *server, LocationScope *location);
    void                        setLanguage(std::vector<std::pair<std::string, float> > languages);
    void                        setKeywordDatabase(DataBase<Variable<std::string> > keywordDatabase);
    void                        keywordFill();


    void                        createResponse(Request *request, ServerScope *server, LocationScope *location, std::string raw);
    std::string                 notAllowed();
    std::string                 writeHeader(void);
    std::string                 errorHtml();
    void                        readContent();
    std::string                 selectIndex();

    void                        GET_method(Request* request, ServerScope* server);
    void	                    POST_method(Request* request, ServerScope* server);
    void	                    DELETE_method();

    std::string         getLink(std::string const &dirEntry, std::string const &dirName);
    std::string         getPage();


};
