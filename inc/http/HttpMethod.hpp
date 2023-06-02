#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "../database/DataBase.hpp"
#include "../entity/IRequest.hpp"

class HttpMethod
{
    private:
        std::vector<std::string> _methods;
        void initMethods();
    public:
        HttpMethod();
        ~HttpMethod();
        HttpMethod(const HttpMethod &requestMethod);

        std::vector<std::string>    get();
        void                        set(std::string method);
        bool                        checkMethodName(std::string methodName);
};
