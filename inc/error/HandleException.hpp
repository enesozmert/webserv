#pragma once

#include <iostream>
#include <exception>
#include "IException.hpp"

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
            std::cerr << ex.what() << std::endl;
            exit(-1);
        }
        catch (...)
        {
            std::cerr << "Unknown exception caught" << std::endl;
            exit(-1);
        }
    }
};