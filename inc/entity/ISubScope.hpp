#pragma once

#include <string>

class ISubScope
{
    public:
        std::string subName;
    public:
        ISubScope() {}
        virtual ~ISubScope() {}
};