#include "../inc/http/HttpMethod.hpp"

HttpMethod::HttpMethod()
{
    this->initMethods();
}

HttpMethod::~HttpMethod(){}

HttpMethod::HttpMethod(const HttpMethod &httpMethod)
{
    *this = httpMethod;
}

std::vector<std::string> HttpMethod::get()
{
    return (this->_methods);
}

void HttpMethod::set(std::string method)
{
    this->_methods.push_back(method);
}

void HttpMethod::initMethods()
{
    this->set("POST");
    this->set("GET");
    this->set("DELETE");
    this->set("UPDATE");
    this->set("PUT");
    this->set("HEAD");
    this->set("OPTIONS");
    this->set("TRACE");
    this->set("CONNECT");
    this->set("PATCH");
}

bool HttpMethod::checkMethodName(std::string methodName)
{
    std::vector<std::string>::iterator it = std::find(this->_methods.begin(), this->_methods.end(), methodName);
    if (it != this->_methods.end() && it->length() == methodName.length())
    {
        return (true);
    }
    return (false);
}