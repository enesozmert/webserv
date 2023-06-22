#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <string>

template <typename T, typename K>
class HttpVariable
{
    private:
        std::pair<T, K> _variable;
    public:
        HttpVariable() {}
        HttpVariable(T key, K value) {_variable = std::make_pair(key, value);}
        ~HttpVariable() {}
    
        T       getKey() const {return (_variable.first);}
        K       getValue() {return (_variable.second);}
        void    setKey(T key){_variable.first = key;}
        void    setValue(K value){_variable.second = value;}
};