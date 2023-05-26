#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <string>

template <typename T, typename K>
class ExceptionVariable
{
    private:
        std::pair<T, K> _variable;
    public:
        ExceptionVariable(){}
        ExceptionVariable(T errorCode, K errorMessage) {_variable = std::make_pair(errorCode, errorMessage);}
        ~ExceptionVariable() {}
    
        T       getErrorCode() const {return (_variable.first);}
        K       getErrorMessage() {return (_variable.second);}
        void    setErrorCode(T errorCode){_variable.first = errorCode;}
        void    setErrorMessage(K errorMessage){_variable.second = errorMessage;}
};