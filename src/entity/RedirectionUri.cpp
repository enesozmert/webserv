#include "../inc/entity/RedirectionUri.hpp"

RedirectionUri::RedirectionUri()
{
}

RedirectionUri::~RedirectionUri()
{
}

RedirectionUri::RedirectionUri(const RedirectionUri &redirectionUri)
{
    *this = redirectionUri;
}

RedirectionUri &RedirectionUri::operator=(const RedirectionUri &redirectionUri)
{
    if (this == &redirectionUri)
        return (*this);
    this->_redirectionCode = redirectionUri._redirectionCode;
    this->_redirectionValue = redirectionUri._redirectionValue;
    return (*this);
}

std::string RedirectionUri::getRedirectionUri()
{
    return (this->_redirectionUri);
}

std::string RedirectionUri::getRedirectionCode()
{
    std::string result;
    std::stringstream sp(this->_redirectionUri);

    sp >> std::ws >> result;
    bool isResult = this->_httpStatusCode.checkErrorCode(this->_redirectionCode);
    result = isResult ? this->_redirectionCode : "none";
    return (result);
}

std::string RedirectionUri::getRedirectionValue()
{
    std::string tempCode;
    std::string value;
    std::stringstream sp(this->_redirectionUri);

    sp >> tempCode;
    sp >> value;
    this->_redirectionValue = value;
    return (this->_redirectionValue);
}

void RedirectionUri::setRedirectionUri(std::string redirectionUri)
{
    this->_redirectionUri = redirectionUri;
}

void RedirectionUri::setRedirectionCode(std::string code)
{
    this->_redirectionCode = code;
}

void RedirectionUri::setRedirectionValue(std::string value)
{
    this->_redirectionValue = value;
}
