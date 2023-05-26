#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>

class IScope
{
    public:
        std::string name;
    public:
        IScope() {}
        virtual ~IScope() {}
        virtual std::string getName() const = 0;
        virtual IScope*     cloneNew() const = 0;
        virtual IScope*     clone() const = 0;
};