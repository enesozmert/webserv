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
    std::map<int, std::string>	        _errorMap;
    std::map<int, std::string>	        _errors;
    std::vector<std::string>	        _indexs;
    std::string					        _path;// local path for request
	std::string					        _cgi_pass;
    std::string					        _error_page;
    std::map<std::string, std::string>	_cgi_params;


    std::string					_allow;
    std::string					_allow_methods;
	std::string					_contentLanguage;
	std::string					_contentLength;
	std::string					_contentLocation;// public part of the path
	std::string					_contentType;
	std::string					_date;
	std::string					_lastModified;
	std::string					_location;
	std::string					_retryAfter;
	std::string					_server;
	std::string					_transferEncoding;//Transfer-Encoding: chunked, Transfer-Encoding: identity
	std::string                 _wwwAuthenticate;
  
public:
    Response();
    ~Response();

    //Getters
    std::string                 getResponse();
    int                         getStatusCode();
    std::map<int, std::string>  getErrorMap();
    std::string                 getHeader();
    std::string                 getStatusMessage();

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
    void                        setErrors();
    void                        setValues();
    void	                    setIndexs(std::vector<std::string> _locationIndex, std::vector<std::string> _serverIndex);
    void                        setParams(std::vector<std::string> _paramKeyword, std::vector<std::string> _paramValue);

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
