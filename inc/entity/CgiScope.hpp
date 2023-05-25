#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "ErrorPage.hpp"
#include "IScope.hpp"
#include "Variable.hpp"
#include "../utils/Utils.hpp"
class CgiScope
{
    protected:
        std::string _pass;
        std::string _param;
        std::string _paramKeyword;
        std::vector<std::string> _paramValues;
    public:
        CgiScope();
        ~CgiScope();
        CgiScope(const CgiScope &cgiScope);
        CgiScope &operator=(const CgiScope &cgiScope);
    
        std::string getPass();
        std::string getParam();
        std::string getParamKeyword();
        std::vector<std::string> getParamValues();
    
        void setPass(std::string pass);
        void setParam(std::string param);
        void setParamKeyword(std::string keyword);
        void setParamValues(std::vector<std::string> values);
    
        // std::string getName() const;
        // CgiScope *cloneNew() const;
        // CgiScope *clone() const;
};