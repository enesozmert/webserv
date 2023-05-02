#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>

class IScope
{
    protected:
        std::string name;
    public:
        virtual ~IScope() {}
        virtual std::string getName() const = 0;
        virtual IScope* clone() const = 0;
};