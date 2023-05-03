#pragma once

#include "../entity/Variable.hpp"

inline bool IsVariableNameEqual(Variable<std::string> variable)
{
    return (variable.getName() == "root");
}