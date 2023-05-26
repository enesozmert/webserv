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

std::string CgiScope::getPass()
{
    return (this->_pass);
}
std::string CgiScope::getParam()
{
    return (this->_param);
}
/* std::vector<std::string>    CgiScope::getParamKeyword()
{
    std::istringstream iss(this->_param);
    std::string keyword;

    iss >> std::ws >> keyword;
    return (keyword);
} */
std::vector<std::string>    CgiScope::getParamValues()
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

void CgiScope::setPass(std::string pass)
{
    this->_pass = pass;
}
void CgiScope::setParam(std::string param)
{
    this->_param = param;
}
void CgiScope::setParamKeyword(std::string keyword)
{
    this->_paramKeyword = keyword;
}
void CgiScope::setParamValues(std::vector<std::string> values)
{
    this->_paramValues = values;
}