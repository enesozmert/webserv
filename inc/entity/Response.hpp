#pragma once

#include "../entity/HttpScope.hpp"
#include "../entity/Request.hpp"

class Response
{
private:
    std::string _response;
    int         statusCode;
    bool        _isAutoIndex;
  
public:
    Response();
    ~Response();

    //Getters
    std::string getResponse();
    int         getStatusCode();
    
    void    createResponse(Request *request, ServerScope *server, LocationScope *location);
};