#pragma once

class IException {
public:
    virtual ~IException() {}
    virtual const char* what() const throw() = 0;
};
