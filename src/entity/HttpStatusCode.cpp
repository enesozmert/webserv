#include "../inc/entity/HttpStatusCode.hpp"

HttpStatusCode::HttpStatusCode()
{
    this->initErrorCodes();
}

HttpStatusCode::~HttpStatusCode() {}

HttpStatusCode::HttpStatusCode(const HttpStatusCode &httpErrorCode)
{
    *this = httpErrorCode;
}

std::vector<std::string> HttpStatusCode::get()
{
    return (this->_errorCodes);
}
        
void HttpStatusCode::set(std::string code)
{
    this->_errorCodes.push_back(code);
}
        
bool HttpStatusCode::checkErrorCode(std::string errorCode)
{
    std::vector<std::string>::iterator it = std::find(this->_errorCodes.begin(), this->_errorCodes.end(), errorCode);
    if (it != this->_errorCodes.end() && it->length() == errorCode.length())
    {
        return (true);
    }
    return (false);
}
        
void HttpStatusCode::initErrorCodes()
{
    this->set("200");
    this->set("201");
    this->set("204");
    this->set("205");
    this->set("301");
    this->set("303");
    this->set("307");
    this->set("400");
    this->set("401");
    this->set("403");
    this->set("404");
    this->set("405");
    this->set("408");
    this->set("410");
    this->set("411");
    this->set("413");
    this->set("414");
    this->set("500");
    this->set("503");
    this->set("504");
    this->set("505");

}