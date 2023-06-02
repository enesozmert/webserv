#pragma once

#include "../entity/HttpScope.hpp"
#include "../entity/Request.hpp"
#include "../utils/Utils.hpp"
#include "../cgi/Cgi.hpp"

#include <list>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <dirent.h>


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
	std::string					        _cgi_pass;
    std::string					        _error_page;
    std::map<std::string, std::string>	_cgi_params;
    std::string					        _method;
    std::string					        _allow_methods;
	std::string					        _contentLanguage;
	std::string					        _contentLength;
	std::string					        _contentLocation;
	std::string					        _contentType;
	std::string					        _date;
	std::string					        _lastModified;
	std::string					        _server;
    int                                 _clientBodybufferSize;
    std::string					        _body;


  
public:
    Response();
    ~Response();

    int                         getStatusCode();
    std::string                 getHeader();
    std::string                 getStatusMessage();
    std::string                 getPath();
    std::string                 getBody();
    std::string                 getMethod();
    std::string                 getCgiPass();

    void                        setAllowMethods(std::vector<std::string> methods);
    void	                    setContentType();
    void	                    setDate();
    void	                    setLastModified();
    void                        setErrors();
    void	                    setIndexs(std::vector<std::string> _locationIndex, std::vector<std::string> _serverIndex);
    void                        setStaticErrorPage();
    int                         setPaths(ServerScope *server, LocationScope *location);
    void                        setClientBodyBufferSize(std::string bodyBufferSize);
    int                         setResponse(Request *request, ServerScope *server, LocationScope *location);

    void                        createResponse(Request *request, ServerScope *server, LocationScope *location);
    std::string                 notAllowed();
    std::string                 writeHeader(void);
    std::string                 readHtml();
    void                        readContent();
    std::string                 selectIndex();

    void                        GET_method(Request* request, ServerScope* server);
    void	                    POST_method(Request* request, ServerScope* server);
    void	                    DELETE_method();
};
