#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "../database/DataBase.hpp"
#include "IRequest.hpp"
#include "Variable.hpp"

class HttpMethod
{
    private:
        std::vector<std::string> _methods;
    public:
        HttpMethod();
        ~HttpMethod();
        HttpMethod(const HttpMethod &requestMethod);

        std::vector<std::string> get();
        void set();
};
