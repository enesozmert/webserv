#include "../inc/error/ClientException.hpp"

ClientException::ClientException()
{
    errorFill();
}

ClientException::~ClientException() {}

ClientException::ClientException(const ClientException &clientException)
{
    (*this) = clientException;
}

ClientException::ClientException(int errorCode)
{
    errorFill();
    this->_errorCode = errorCode;
}

DataBase<ExceptionVariable<int, std::string> > ClientException::getErrorDataBase()
{
    return (this->_errorDatabase);
}
void ClientException::setErrorDatabase(DataBase<ExceptionVariable<int, std::string> > errorDatabase)
{
    this->_errorDatabase = errorDatabase;
}
void ClientException::errorFill()
{
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(300, "No matched server/location found"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(301, "There is no possible server"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(302, "There is no possible location"));
}

void ClientException::exceptionHandler()
{
    throw MyException("Excepiton : " + this->_exception.getErrorMessage());
}

void ClientException::run(int errorCode)
{
    if (errorCode == -1)
        errorCode = this->_errorCode;
    if (this->getErrorDataBase().isHere<int, IsExceptionVariableErrorCodeEqual>(errorCode))
    {
        this->_exception = this->getErrorDataBase().getByNameData<int, IsExceptionVariableErrorCodeEqual>(errorCode);
        HandleException::Error(*this, &ClientException::exceptionHandler);
    }
}