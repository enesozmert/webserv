#pragma once

#include "../entity/HttpScope.hpp"
#include "../entity/Request.hpp"
#include "../utils/Utils.hpp"
#include "../cgi/Cgi.hpp"

#include <list>
# include <sys/time.h>

class Response
{
private:
    std::string _response;
    int         statusCode;
    bool        _isAutoIndex;

    std::vector<std::string>			_index;//bunu araştır???
    std::string							_path;// local path for request
    std::map<std::string, std::string>	_cgi_param;
	std::string							_cgi_pass;


    std::string					_allow;
	std::string					_contentLanguage;
	std::string					_contentLength;
	std::string					_contentLocation;// public part of the path
	std::string					_contentType;
	std::string					_date;
	std::string					_lastModified;
	std::string					_location;
	std::string					_retryAfter;
	std::string					_server;
	std::string					_transferEncoding;
	std::string					_wwwAuthenticate;
  
public:
    Response();
    ~Response();

    //Getters
    std::string getResponse();
    int         getStatusCode();

    //Setters
    void    setAllow(std::vector<std::string> methods);
    void	setAllow(const std::string& allow);
    void	setContentLanguage(const std::string& lang);
    void	setContentLength(size_t size);
    void	setContentLocation(const std::string& path, int code);
    void	setContentType(std::string type, std::string path);
    void	setDate(void);
    void	setLastModified(const std::string& path);
    void	setLocation(int code, const std::string& redirect);
    void	setRetryAfter(int code, int sec);
    void	setServer(void);
    void	setTransferEncoding(void);
    void	setWwwAuthenticate(int code);
    void    setValues(size_t size, const std::string& path, int code, std::string type, const std::string& contentLocation, const std::string& lang);

    void    resetValues(void);
    
    void    createResponse(Request *request, ServerScope *server, LocationScope *location);
    std::string notAllowed(std::vector<std::string> methods, const std::string& path, int code, const std::string& lang);
    std::string	writeHeader(void);

    void    GETmethod(Request* request, ServerScope* scope, LocationScope* location);
    void	DELETEmethod(Request* request, ServerScope* scope, LocationScope* location);
    void	POSTmethod(Request* request, ServerScope* scope, LocationScope* location);
};
