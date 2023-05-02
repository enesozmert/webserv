#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <iterator>
#include "../file/File.hpp"
#include "../entity/Http.hpp"
#include "../entity/ParseLineProp.hpp"

class File;
class Http;
class ParseLineProp;
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
        void parseScope(const std::vector<std::string> &lines);
        void parseMatchClass(std::vector<ParseLineProp> parseLineProps);

        std::vector<std::string> getScopeOrderNames(const std::vector<std::string> &lines);
        size_t getScopeOrderNameCount(std::vector<std::string> scopeNames, std::string scopeName);
        size_t findClosingScopeIndex(const std::vector<std::string>& lines, size_t startingIndex);

        void parserLocationPath(const std::string &location);
        void location(std::vector<std::string> tempScopes);
        void server(std::vector<std::string> tempScopes);
        void http(std::vector<std::string> tempScopes);
};

