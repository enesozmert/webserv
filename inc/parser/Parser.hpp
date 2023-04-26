#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../file/File.hpp"

class File;
class Parser
{
    private:
    public:
        Parser();
        ~Parser();
        void parse(std::string &fileName);
};

