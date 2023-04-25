#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>

class ErrorPage
{
    private:
        std::vector<std::string> _codes;
        std::string _pageName;
    public:
        ErrorPage(/* args */);
        ~ErrorPage();
    
        std::vector<std::string> getCodes();
        std::string getPageName();
        void setCodes(std::string code);
        void setPageName(std::string pageName);
};