#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class File
{
    private:
        std::ifstream _file;
    public:
        File();
        ~File();
        File(const std::string& fileName);
        std::string readToString();
        void close();
};
