#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <iterator>
#include "../file/File.hpp"
#include "../entity/Http.hpp"

class File;
class Http;
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
        void parserScope(const std::vector<std::string> &lines);

        void updateValueUsingAt(std::map<std::string, bool>& myMap, const std::string& key, bool newValue);

        void parserLocationPath(const std::string &location);
        void location(std::vector<std::string> tempScopes);
        void server(std::vector<std::string> tempScopes);
        void http(std::vector<std::string> tempScopes);
};

