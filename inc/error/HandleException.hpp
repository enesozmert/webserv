#pragma once

#include <iostream>
#include <exception>
#include "IException.hpp"

class HandleException
{
public:
    static std::string HandleExMessage;

    template <typename T>
    static std::string Error(T action)
    {
        try
        {
            action();
        }
        catch (const IException &ex)
        {
            HandleExMessage = ex.what();
        }
        catch (...)
        {
            HandleExMessage = "Unknown exception caught";
        }

        return HandleExMessage;
    }
};

std::string HandleException::HandleExMessage = "";