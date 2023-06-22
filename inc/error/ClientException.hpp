#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "../database/DataBase.hpp"
#include "../function/ExceptionVariableFunc.hpp"
#include "MyException.hpp"
#include "HandleException.hpp"

class ClientException
{
    private:
        int _errorCode;
        DataBase<ExceptionVariable<int, std::string> > _errorDatabase;
        ExceptionVariable<int, std::string> _exception;
    public:
        ClientException();
        ~ClientException();
        ClientException(const ClientException &clientException);
        ClientException(int errorCode);

        DataBase<ExceptionVariable<int, std::string> > getErrorDataBase();
        void setErrorDatabase(DataBase<ExceptionVariable<int, std::string> > errorDatabase);
        void exceptionHandler();
    private:
        void errorFill();
    public:
        void run(int errorCode = -1);
};