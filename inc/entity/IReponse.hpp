#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>

class IResponse
{
    public:
        std::string name;
    public:
        IResponse() {}
        virtual ~IResponse() {}
        virtual std::string     getName() const = 0;
        virtual IResponse*      cloneNew() const = 0;
        virtual IResponse*      clone() const = 0;
};