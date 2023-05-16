#pragma once

#include "../entity/ExceptionVariable.hpp"

struct IsExceptionVariableErrorCodeEqual : public std::unary_function<ExceptionVariable<int, std::string>, bool>
{
    IsVariableNameEqual(const int &errorCode) : _errorCode(errorCode) {}

    bool operator()(const ExceptionVariable<int, std::string> &exceptionVariable) const
    {
        return (variable.getErrorCode() == _errorCode);
    }

    int _errorCode;
};