#include "../inc/error/ConfigException.hpp"

ConfigException::ConfigException()
{
    errorFill();
}

ConfigException::~ConfigException() {}

ConfigException::ConfigException(const ConfigException &configException)
{
    (*this) = configException;
}

ConfigException::ConfigException(int errorCode)
{
    errorFill();
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
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(105, "Not Found file extension."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(106, "Invalid argument."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(107, "Please provide a config file."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(108, "Bracket is unbalanced."));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(109, "Expected 'server'"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(110, "Expected 'location'"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(111, "Semicolon not found ';'"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(112, "Convert Exception"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(113, "Http scope count not equal to one"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(114, "Server scope count zero"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(115, "Http Convert Error"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(116, "Server Convert Error"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(117, "Location Convert Error"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(118, "Invalid content in file"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(119, "More than one consecutive space in a line"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(120, "You cannot put a space before a semicolon"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(121, "You cannot put a space after a semicolon"));
    _errorDatabase.insertData(ExceptionVariable<int, std::string>(122, "You cannot put a space at the beginning of a line"));
}

void ConfigException::exceptionHandler()
{
    throw MyException("Excepiton : " + this->_exception.getErrorMessage());
}

void ConfigException::run(int errorCode)
{
    if (errorCode == -1)
        errorCode = this->_errorCode;
    if (this->getErrorDataBase().isHere<int, IsExceptionVariableErrorCodeEqual>(errorCode))
    {
        this->_exception = this->getErrorDataBase().getByNameData<int, IsExceptionVariableErrorCodeEqual>(errorCode);
        HandleException::Error(*this, &ConfigException::exceptionHandler);
    }
}