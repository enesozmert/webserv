#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "../database/DataBase.hpp"
#include "../function/ExceptionVariableFunc.hpp"
#include "MyException.hpp"
#include "HandleException.hpp"

class ServerException
{
    private:
        int _errorCode;
        DataBase<ExceptionVariable<int, std::string> > _errorDatabase;
        ExceptionVariable<int, std::string> _exception;
    public:
        ServerException(/* args */);
        ~ServerException();
        ServerException(const ServerException &serverException);
        ServerException(int errorCode);

        DataBase<ExceptionVariable<int, std::string> > getErrorDataBase();
        void setErrorDatabase(DataBase<ExceptionVariable<int, std::string> > errorDatabase);
        void exceptionHandler();
    private:
        void errorFill();
    public:
        void run(int errorCode = -1);
};