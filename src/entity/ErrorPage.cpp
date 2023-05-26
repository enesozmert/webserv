#include "../inc/entity/ErrorPage.hpp"

ErrorPage::ErrorPage()
{
}

ErrorPage::~ErrorPage()
{
}

ErrorPage::ErrorPage(const ErrorPage &errorPage)
{
    *this = errorPage;
}

ErrorPage	&ErrorPage::operator=(const ErrorPage &errorPage)
{
    if (this == &errorPage)
        return (*this);
    this->_errorPageCodes = errorPage._errorPageCodes;
    this->_errorPagePath = errorPage._errorPagePath;
    return (*this);
}

std::vector<std::string> ErrorPage::getErrorPageCodes()
{
    std::istringstream iss(this->_errorPage);

    std::string token;
    while (iss >> std::ws >> token) {
        bool isNumber = true;
        for (std::string::size_type i = 0; i < token.length(); ++i) {
            if (!std::isdigit(token[i])) {
                isNumber = false;
                break;
            }
        }

        if (isNumber && _httpStatusCode.checkErrorCode(token)) {
            this->_errorPageCodes.push_back(token);
        }
    }
    return (this->_errorPageCodes);
}

std::string ErrorPage::getErrorPagePath()
{
    std::istringstream iss(this->_errorPage);

    std::string token;
    while (iss >> std::ws >> token) {
        bool isNumber = true;
        for (std::string::size_type i = 0; i < token.length(); ++i) {
            if (!std::isdigit(token[i])) {
                isNumber = false;
                break;
            }
        }

        if (!isNumber) {
            this->_errorPagePath = token;
        }
    }
    return (this->_errorPagePath);
}

std::string ErrorPage::getErrorPage()
{
    return (this->_errorPage);
}

void ErrorPage::setErrorPageCodes(std::string code)
{
    this->_errorPageCodes.push_back(code);
}

void ErrorPage::setErrorPagePath(std::string pagePath)
{
    this->_errorPagePath = pagePath;
}

void ErrorPage::setErrorPage(std::string errorPage)
{
    this->_errorPage = errorPage;
}
