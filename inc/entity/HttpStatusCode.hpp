#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
class HttpStatusCode
{
    private:
        std::vector<std::string> _errorCodes;
    public:
        HttpStatusCode();
        ~HttpStatusCode();
        HttpStatusCode(const HttpStatusCode &httpErrorCode);

        std::vector<std::string> get();
        void set(std::string code);

        bool checkErrorCode(std::string errorCode);
    private:
        void initErrorCodes();
};
