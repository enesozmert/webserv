#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "ISubScope.hpp"
#include "../http/HttpStatusCode.hpp"

class HttpStatusCode;
class RedirectionUri : public ISubScope
{
    protected:
        std::string _redirectionUri;
        std::string _redirectionCode;
        std::string _redirectionValue;
        HttpStatusCode _httpStatusCode;
    public:
        RedirectionUri();
        ~RedirectionUri();
        RedirectionUri(const RedirectionUri &redirectionUri);
        RedirectionUri	&operator=(const RedirectionUri &redirectionUri);

        std::string getRedirectionCode();
        std::string getRedirectionValue();
        std::string getRedirectionUri();

        void setRedirectionCode(std::string code);
        void setRedirectionValue(std::string value);
        void setRedirectionUri(std::string redirectionUri); 
};
