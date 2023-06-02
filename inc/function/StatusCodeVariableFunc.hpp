#pragma once

#include "../entity/HttpVariable.hpp"

struct IsStatusCodeVariableStatusCodeEqual : public std::unary_function<HttpVariable<int, std::string>, bool>
{
    IsStatusCodeVariableStatusCodeEqual(const int statusCode) : _statusCode(statusCode) {}

    bool operator()(const HttpVariable<int, std::string> &httpVariable) const
    {
        return (httpVariable.getKey() == _statusCode);
    }

    int _statusCode;
};