#include "../inc/error/ServerException.hpp"

ServerException::ServerException()
{
    errorFill();
}

ServerException::~ServerException() {}

ServerException::ServerException(const ServerException &serverException)
{
    (*this) = serverException;
}

ServerException::ServerException(int errorCode)
{
    errorFill();
    this->_errorCode = errorCode;
}

DataBase<ExceptionVariable<int, std::string> > ServerException::getErrorDataBase()
{
    return (this->_errorDatabase);
}
void ServerException::setErrorDatabase(DataBase<ExceptionVariable<int, std::string> > errorDatabase)
{
    this->_errorDatabase = errorDatabase;
}
void ServerException::errorFill()
{
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(200, "Could not create server."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(201, "Could not re addr."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(202, "Could not bind port"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(203, "Could not listen."));
}

void ServerException::exceptionHandler()
{
    throw MyException("Excepiton : " + this->_exception.getErrorMessage());
}

void ServerException::run(int errorCode)
{
    if (errorCode == -1)
        errorCode = this->_errorCode;
    if (this->getErrorDataBase().isHere<int, IsExceptionVariableErrorCodeEqual>(errorCode))
    {
        this->_exception = this->getErrorDataBase().getByNameData<int, IsExceptionVariableErrorCodeEqual>(errorCode);
        HandleException::Error(*this, &ServerException::exceptionHandler);
    }
}
