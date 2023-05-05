#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <iterator>
#include "../file/File.hpp"
#include "../database/DataBase.hpp"
#include "../entity/Http.hpp"
#include "../entity/Server.hpp"
#include "../entity/Location.hpp"
#include "../entity/ParseLineProp.hpp"
#include "../entity/IScope.hpp"
#include "../entity/Variable.hpp"
#include "../function/ParseLinePropFunc.hpp"
#include "../function/VariableFunc.hpp"

class File;
class Http;
class Server;
class Location;
class ParseLineProp;
class Parser
{
    private:
        Http http;
        Server server;
        Location location;
        Http *httpPtr;
        Server *serverPtr;
        Location *locationPtr;
        std::map<size_t, ParseLineProp> _parseLineProps;
        std::vector<ParseLineProp> _orderParseLineProps;
        std::vector<std::string> _scopeNames;
        std::vector<std::string> _orderScopeNames;
        std::map<size_t, IScope *> _matchedClass;

        size_t _matchedClassIndex;
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
        void parseMatchClass();
        void parseMatchedClassFill(std::map<size_t, IScope *> matchedClass);

        void setScopeNames(const std::vector<std::string> &lines);
        std::vector<std::string> getScopeNames();

        void setOrderScopeNames();

        size_t getScopeOrderNameCount(std::vector<std::string> scopeNames, std::string scopeName);
        size_t findClosingScopeIndex(const std::vector<std::string> &lines, size_t startingIndex);
        size_t findOpeningScopeIndex(const std::vector<std::string> &lines, size_t closingIndex);

        void parserLocationPath(const std::string &location);

        void parseLocation(std::vector<size_t> tempScopes);
        void parseServer(std::vector<size_t> tempScopes);
        void parseHttp(std::vector<size_t> tempScopes);
};