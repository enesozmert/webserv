#include "../inc/error/ConfigException.hpp"

ConfigException::ConfigException(/* args */)
{
}

ConfigException::~ConfigException()
{
}

ConfigException::ConfigException(const ConfigException &configException)
{
    (*this) = configException;
}

ConfigException::ConfigException(int errorCode)
{
    this->_errorCode = errorCode;
}

DataBase<ExceptionVariable<int, std::string> > ConfigException::getErrorDataBase()
{
    return (this->_errorDatabase);
}
void ConfigException::setErrorDatabase(DataBase<ExceptionVariable<int, std::string> > errorDatabase)
{
    this->_errorDatabase = errorDatabase;
}
void ConfigException::errorFill()
{
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(100, "Config file argument not found."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(101, "Could not open config file."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(102, "Config file is empty"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(103, "Config file name is empty"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(104, "Invalid file extension."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(105, "Invalid argument."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(106, "Please provide a config file."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(107, "Parenthesis is unbalanced."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(108, "Expected 'server'"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(109, "Expected 'location'"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(110, "Location must be end with ';'"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(111, "Convert Exception"));
}

void ConfigException::exceptionHandler()
{
    throw MyException(this->_exception.getErrorMessage());
}

void ConfigException::run(int errorCode)
{
    if (errorCode == -1)
        errorCode = this->_errorCode;
    if (this->getErrorDataBase().isHere<int, IsExceptionVariableErrorCodeEqual>(errorCode))
    {
        this->_exception = this->getErrorDataBase().getByNameData<int, IsExceptionVariableErrorCodeEqual>(errorCode);
        std::cerr << this->_exception.getErrorCode() << std::endl;
        HandleException::Error(&ConfigException::exceptionHandler);
        abort();
    }
}