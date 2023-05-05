#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>

template <typename T>
class Variable
{
    private:
        T _name;
        T* _value;
    public:
        Variable(){}
        Variable(T name, T* value) : _name(name), _value(value)  {}
        ~Variable() {}
        T getName() const {return (_name);}
        T getValue() const {return (_value);}
};