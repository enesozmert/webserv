#pragma once

#include "../entity/HttpVariable.hpp"

struct IsContentTypeTypeEqual : public std::unary_function<HttpVariable<std::string, std::string>, bool>
{
    IsContentTypeTypeEqual(const std::string type) : _type(type) {}

    bool operator()(const HttpVariable<std::string, std::string> &httpVariable) const
    {
        return (httpVariable.getKey() == _type);
    }

    std::string _type;
};