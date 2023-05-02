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
#include "../entity/Server.hpp"
#include "../entity/Location.hpp"
#include "../entity/ParseLineProp.hpp"
#include "../entity/IScope.hpp"
// #include "../function/ParseLinePropFunc.hpp"

class File;
class Http;
class Server;
class Location;
class ParseLineProp;
class Parser
{
    private:
        std::map<size_t, ParseLineProp *> _parseLineProps;
        // std::vector<std::string> _lines;
        // std::vector<std::string> _linesNotScope;
    public:
        Parser();
        ~Parser();


        std::string cleanString(std::string str);
        std::string trim(const std::string& str, const std::string& delimiters);
        std::vector<std::string> splitString(const std::string &str, const char delimiter);

        void parse(std::string &fileName);

        Server serverParse(size_t i, std::vector<std::string> lines);
        Location locationParse(size_t i, std::vector<std::string> lines);


        void printVec(std::vector<std::string> vec);

        void parseScope(const std::vector<std::string> &lines);

        void parseScopeFill();

        void parseMatchClass(const std::vector<std::string> &lines);

        void parseMatchedClassFill(std::map<size_t, IScope *> matchedClass);

        std::vector<std::string> getScopeOrderNames(const std::vector<std::string> &lines);
        size_t getScopeOrderNameCount(std::vector<std::string> scopeNames, std::string scopeName);
        size_t findClosingScopeIndex(const std::vector<std::string> &lines, size_t startingIndex);
        size_t findOpeningScopeIndex(const std::vector<std::string> &lines, size_t closingIndex);

        void parserLocationPath(const std::string &location);
        void location(std::vector<std::string> tempScopes);
        void server(std::vector<std::string> tempScopes);
        void http(std::vector<std::string> tempScopes);
};