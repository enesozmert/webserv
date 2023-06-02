#pragma once

#include <iostream>
#include <exception>
#include <cstdlib>
#include "IException.hpp"
#include "../utils/Utils.hpp"

class HandleException
{
public:
    template <typename T>
    static void Error(T& obj, void (T::*func)())
    {
        try
        {
            (obj.*func)();
        }
        catch (const IException &ex)
        {
            std::cerr << RED << ex.what() << RESET << std::endl;
            exit(-1);
        }
        catch (...)
        {
            std::cerr << RED << "Unknown exception caught" << RESET << std::endl;
            exit(-1);
        }
    }
};