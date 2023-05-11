#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "../database/DataBase.hpp"
#include "IRequest.hpp"
#include "Variable.hpp"

class RequestMethod
{
    private:
        std::vector<std::string> _methods;
    public:
        RequestMethod();
        ~RequestMethod();
        RequestMethod(const RequestMethod &requestMethod);

        std::vector<std::string> get();
        void set();
};
