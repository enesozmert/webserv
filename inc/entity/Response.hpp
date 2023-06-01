#pragma once

#include <list>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include "../entity/HttpScope.hpp"
#include "../http/HttpStatusCode.hpp"
#include "../http/HttpContentType.hpp"
#include "../entity/Request.hpp"
#include "../utils/Utils.hpp"
#include "../cgi/Cgi.hpp"

class Response
{
private:
    std::string                         _response;
    int                                 statusCode;
    std::string					        _type;
    std::map<int, std::string>	        _errorMap;
    std::vector<std::string>	        _indexs;
    std::string					        _path;
	std::string					        _cgi_pass;
    std::string					        _error_page;
    std::map<std::string, std::string>	_cgi_params;


    std::string					_allow;
    std::string					_allow_methods;
	std::string					_contentLanguage;
	std::string					_contentLength;
	std::string					_contentLocation;
	std::string					_contentType;
	std::string					_date;
	std::string					_lastModified;
	std::string					_location;
	std::string					_retryAfter;
	std::string					_server;
	std::string					_transferEncoding;
	std::string                 _wwwAuthenticate;
    HttpContentType             _httpContentType;
    HttpStatusCode              _httpStatusCode;
public:
    Response();
    ~Response();

    //Getters
    std::string                 getResponse();
    int                         getStatusCode();
    std::map<int, std::string>  getErrorMap();
    std::string                 getHeader();

    //Setters
    void                        setAllowMethods(std::vector<std::string> methods);
    void	                    setContentLength();
    void	                    setContentType();
    void	                    setDate();
    void	                    setLastModified();
    void	                    setLocation();
    void	                    setRetryAfter();
    void	                    setServer();
    void	                    setTransferEncoding();
    void	                    setWwwAuthenticate();
    void                        setErrorMap();
    void                        setValues();
    void	                    setIndexs(std::vector<std::string> _locationIndex, std::vector<std::string> _serverIndex);
    void                        setParams(std::vector<std::string> _paramKeyword, std::vector<std::string> _paramValue);
    void                        setLanguage(std::vector<std::pair<std::string, float> > languages);

    void                        resetValues();
    void                        createResponse(Request *request, ServerScope *server, LocationScope *location);
    std::string                 notAllowed(std::vector<std::string> methods);
    std::string                 writeHeader(void);
    std::string                 readHtml();
    void                        readContent();
    std::string                 selectIndex();

    void    GETmethod(Request* request, ServerScope* server);
    void	DELETEmethod();
    void	POSTmethod(Request* request, ServerScope* server);
};
