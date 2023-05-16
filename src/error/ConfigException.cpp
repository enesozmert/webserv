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

DataBase<Variable<std::string> > ConfigException::getErrorDataBase()
{
    return (this->_errorDatabase);
}
void ConfigException::setErrorDatabase(DataBase<Variable<std::string> > errorDatabase)
{
    this->_errorDatabase = errorDatabase;
}
void ConfigException::errorFill()
{
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(100, "Config file argument not found."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(101, "Could not open config file."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(101, "Config file is empty"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(102, "Invalid file extension."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(103, "Invalid argument."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(104, "Please provide a config file."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(105, "Parenthesis is unbalanced."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(106, "Expected 'server'"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(107, "Expected 'location'"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(108, "Location must be end with ';'"));
}

void ConfigException::run()
{
    if (this->getErrorDataBase().isHere<IsExceptionVariableErrorCodeEqual>(tempVariableName))
        {
            this->getErrorDataBase().updateData<IsExceptionVariableErrorCodeEqual, std::string>(tempVariableName, tempVariableValue);
        }
}