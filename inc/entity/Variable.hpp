#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <string>

template <typename T>
class Variable
{
    private:
        std::pair<T, T*> _variable;
    public:
        Variable(){}
        Variable(T name, T* value) {_variable = std::make_pair(name, value);}
        ~Variable() {}
    
        T       getName() const {return (_variable.first);}
        T*      getValue() const {return (_variable.second);}
        void    setName(T name){_variable.first = name;}
        void    setValue(T* value){_variable.second = value;}
};