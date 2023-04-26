#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../file/File.hpp"

class File;
class Parser
{
    private:
    public:
        Parser();
        ~Parser();


        std::string cleanString(std::string str);
        std::string trim(const std::string& str, const std::string& delimiters);
        std::vector<std::string> splitString(const std::string& str, const char delimiter);
        void parserLocationPath(const std::string &location);
        void parse(std::string &fileName);
};

