#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "ErrorPage.hpp"
#include "ISubScope.hpp"
#include "Variable.hpp"
#include "../utils/Utils.hpp"

class CgiScope : public ISubScope
{
    protected:
        std::string                 _pass;
        std::string                 _param;
        std::string                 _paramKeyword;
        std::vector<std::string>    _paramValues;
    public:
        CgiScope();
        ~CgiScope();
        CgiScope(const CgiScope &cgiScope);
        CgiScope &operator=(const CgiScope &cgiScope);

        std::vector<std::string>    getCgiPass();
        std::string                 getCgiParam();
        std::string                 getCgiParamKeyword();
        std::vector<std::string>    getCgiParamValues();

        void setCgiPass(std::string pass);
        void setCgiParam(std::string param);
        void setCgiParamKeyword(std::string keyword);
        void setCgiParamValues(std::vector<std::string> values);
};