#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <string>

template <typename T, typename K>
class CgiVariable
{
    private:
        std::pair<T, K> _variable;
    public:
        CgiVariable(){}
        CgiVariable(T key, K value) {_variable = std::make_pair(key, value);}
        ~CgiVariable() {}
    
        T       getKey() const {return (_variable.first);}
        K       getValue() const {return (_variable.second);}
        void    setKey(T key){_variable.first = key;}
        void    setValue(K value){_variable.second = value;}
};