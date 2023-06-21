#include "../inc/error/ClusterException.hpp"

ClusterException::ClusterException()
{
    errorFill();
}

ClusterException::~ClusterException() {}

ClusterException::ClusterException(const ClusterException &clusterException)
{
    (*this) = clusterException;
}

ClusterException::ClusterException(int errorCode)
{
    errorFill();
    this->_errorCode = errorCode;
}

DataBase<ExceptionVariable<int, std::string> > ClusterException::getErrorDataBase()
{
    return (this->_errorDatabase);
}
void ClusterException::setErrorDatabase(DataBase<ExceptionVariable<int, std::string> > errorDatabase)
{
    this->_errorDatabase = errorDatabase;
}
void ClusterException::errorFill()
{
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(400, "Could not setup cluster !"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(401, "Wrong Data !"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(402, "So Big Data !"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(403, "Read error, closing connection."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(404, "Could not create socket."));
}

void ClusterException::exceptionHandler()
{
    throw MyException("Excepiton : " + this->_exception.getErrorMessage());
}

void ClusterException::run(int errorCode)
{
    if (errorCode == -1)
        errorCode = this->_errorCode;
    if (this->getErrorDataBase().isHere<int, IsExceptionVariableErrorCodeEqual>(errorCode))
    {
        this->_exception = this->getErrorDataBase().getByNameData<int, IsExceptionVariableErrorCodeEqual>(errorCode);
        HandleException::Error(*this, &ClusterException::exceptionHandler);
    }
}