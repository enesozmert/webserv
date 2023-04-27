#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../file/File.hpp"
#include "../entity/Server.hpp"
#include "../entity/Location.hpp"
#include "../entity/Http.hpp"

class File;
class Parser
{
    private:
    public:
        Parser();
        ~Parser();
        void parse(std::string &fileName);
        Server serverParse(size_t i, std::vector<std::string> lines);
        Location locationParse(size_t i, std::vector<std::string> lines);


        void printVec(std::vector<std::string> vec);

};

