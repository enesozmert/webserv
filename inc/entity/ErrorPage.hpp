#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "ISubScope.hpp"
#include "Variable.hpp"
#include "../database/DataBase.hpp"
#include "../http/HttpStatusCode.hpp"

class HttpStatusCode;
class ErrorPage : public ISubScope
{
    protected:
        std::vector<std::string>    _errorPageCodes;
        std::string                 _errorPage;
        std::string                 _errorPagePath;
        HttpStatusCode              _httpStatusCode;
    public:
        ErrorPage();
        ~ErrorPage();
        ErrorPage(const ErrorPage &errorPage);
        ErrorPage &operator=(const ErrorPage &errorPage);
    
        std::vector<std::string>    getErrorPageCodes();
        std::string                 getErrorPagePath();
        std::string                 getErrorPage();

        void                        setErrorPageCodes(std::string code);
        void                        setErrorPagePath(std::string pagePath);
        void                        setErrorPage(std::string errorPage);
};