#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "../database/DataBase.hpp"
#include "../entity/ExceptionVariable.hpp"
#include "../function/ExceptionVariableFunc.hpp"

class ConfigException
{
    private:
        int _errorCode;
        DataBase<Variable<std::string> > _errorDatabase;
    public:
        ConfigException(/* args */);
        ~ConfigException();
        ConfigException(const ConfigException &configException);
        ConfigException(int errorCode);

        DataBase<Variable<std::string> > getErrorDataBase();
        void setErrorDatabase(DataBase<Variable<std::string> > errorDatabase);
        void errorFill();
        void run();
};