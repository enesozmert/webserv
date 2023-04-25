#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include "ErrorPage.hpp"
#include "RedirectionUri.hpp"

class ErrorPage;
class RedirectionUri;
class Location
{
    private:
        std::string root;
        bool autoindex;
        std::string index;
        ErrorPage _errorPage;
        RedirectionUri _redirectionUri;
    public:
        Location(/* args */);
        ~Location();
        std::string getRoot();
        bool getAutoindex();
        std::string getIndex();
        ErrorPage getErrorPage();
        RedirectionUri getRedirectionUri();

        void setRoot(std::string root);
        void setIndex(std::string index);
        void setAutoindex(bool autoIndex);
        void setErrorPage(ErrorPage errorPage);
        void setRedirectionUri(RedirectionUri redirectionUri);
};