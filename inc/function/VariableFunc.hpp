#pragma once

#include "../entity/Variable.hpp"

struct IsVariableNameEqual : public std::unary_function<Variable<std::string>, bool>
{
    IsVariableNameEqual(const std::string &name) : _name(name) {}

    bool operator()(const Variable<std::string> &variable) const
    {
        return (variable.getName() == _name);
    }

    std::string _name;
};