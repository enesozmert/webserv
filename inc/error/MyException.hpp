#pragma once
#include <iostream>
#include <string>
#include "IException.hpp"

class MyException : public IException {
private:
    std::string message;
public:
    MyException(const std::string& msg) : message(msg) {}
    const char* what() const throw() {
        return message.c_str();
    }
};
