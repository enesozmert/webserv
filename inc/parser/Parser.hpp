#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
<<<<<<< HEAD
=======
#include <map>
#include <iterator>
>>>>>>> eozmert
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


        std::string cleanString(std::string str);
        std::string trim(const std::string& str, const std::string& delimiters);
        std::vector<std::string> splitString(const std::string &str, const char delimiter);

        void parse(std::string &fileName);
<<<<<<< HEAD
        Server serverParse(size_t i, std::vector<std::string> lines);
        Location locationParse(size_t i, std::vector<std::string> lines);


        void printVec(std::vector<std::string> vec);

=======
        void parserScope(const std::vector<std::string> &lines);

        void parserLocationPath(const std::string &location);
        void location(std::vector<std::string> tempScopes);
        void server(std::vector<std::string> tempScopes);
        void http(std::vector<std::string> tempScopes);
>>>>>>> eozmert
};

