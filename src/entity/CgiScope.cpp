#include "../inc/entity/CgiScope.hpp"

CgiScope::CgiScope() {}

CgiScope::~CgiScope() {}

CgiScope::CgiScope(const CgiScope &cgiScope)
{
    *this = cgiScope;
}
CgiScope &CgiScope::operator=(const CgiScope &cgiScope)
{
    if (this == &cgiScope)
        return (*this);
    this->_pass = cgiScope._pass;
    this->_paramValues = cgiScope._paramValues;
    this->_paramKeyword = cgiScope._paramKeyword;
    this->_param = cgiScope._param;
    return (*this);
}

std::vector<std::string> CgiScope::getCgiPass()
{
    std::istringstream iss(this->_pass);
    std::vector<std::string> result;
    std::string keyword;

    while (iss >> std::ws >> keyword) {
        result.push_back(keyword);
    }
    return (result);
}
std::string CgiScope::getCgiParam()
{
    return (this->_param);
}

std::string CgiScope::getCgiParamKeyword()
{
    std::istringstream iss(this->_param);
    std::string keyword;

    iss >> std::ws >> keyword;
    return (keyword);
}
std::vector<std::string> CgiScope::getCgiParamValues()

{
    std::istringstream iss(this->_param);
    std::vector<std::string> result;
    std::string keyword;
    std::string word;

    iss >> std::ws >> keyword;
    while (iss >> std::ws >> word) {
        result.push_back(word);
    }
    return result;
}

void CgiScope::setCgiPass(std::string pass)
{
    this->_pass = pass;
}
void CgiScope::setCgiParam(std::string param)
{
    this->_param = param;
}
void CgiScope::setCgiParamKeyword(std::string keyword)
{
    this->_paramKeyword = keyword;
}
void CgiScope::setCgiParamValues(std::vector<std::string> values)
{
    this->_paramValues = values;
}