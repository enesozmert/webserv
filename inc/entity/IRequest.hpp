#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>

class IRequest
{
    public:
        std::string name;
    public:
        IRequest() {}
        virtual ~IRequest() {}
        virtual std::string getName() const = 0;
        virtual IRequest*   cloneNew() const = 0;
        virtual IRequest*   clone() const = 0;
};