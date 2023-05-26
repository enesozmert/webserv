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
#include "../entity/HttpScope.hpp"
#include "../entity/ServerScope.hpp"
#include "../entity/LocationScope.hpp"
#include "../entity/ParseLineProp.hpp"
#include "../entity/IScope.hpp"
#include "../entity/Variable.hpp"
#include "../function/ParseLinePropFunc.hpp"
#include "../function/VariableFunc.hpp"
#include "../utils/Utils.hpp"
#include "../error/ConfigException.hpp"

class File;
class HttpScope;
class ServerScope;
class LocationScope;
class ParseLineProp;
class ConfigException;
class ParserConfig
{
    private:
        HttpScope*      _parsedHttp;
        HttpScope       http;
        ServerScope     server;
        LocationScope   location;

        HttpScope*      httpPtr;
        ServerScope*    serverPtr;
        LocationScope*  locationPtr;

        std::vector<std::string>        _lines;
        std::map<size_t, ParseLineProp> _parseLineProps;
        std::vector<ParseLineProp>      _orderParseLineProps;
        std::vector<std::string>        _scopeNames;
        std::vector<std::string>        _orderScopeNames;
        std::map<size_t, IScope *>      _matchedClass;
        size_t                          _matchedClassIndex;

        ConfigException _configException;
    public:
        ParserConfig();
        ~ParserConfig();

        std::string getCheckFileExtension(File &file, std::string &fileName);

        void parse(std::string &fileName);
        void parseSyntax(std::string &fileName);
    private:
        void parseFile(std::string &fileName);
        void parseScope(const std::vector<std::string> &lines);
        void parseMatchClass();
        void parseScopeFill();
        void parseMatchedClassFill();

        void parseHttp(std::vector<size_t> tempScopes);
        void parseServer(std::vector<size_t> tempScopes);
        void parseLocation(std::vector<size_t> tempScopes);

        size_t findClosingScopeIndex(const std::vector<std::string> &lines, size_t startingIndex);
        size_t findOpeningScopeIndex(const std::vector<std::string> &lines, size_t closingIndex);
    public:
        void setScopeNames(const std::vector<std::string> &lines);
        std::vector<std::string> getScopeNames();

        void setOrderScopeNames();
        std::vector<std::string> getOrderScopeNames();

        std::map<size_t, ParseLineProp> getParseLineProps();

        void setOrderParseLineProps();
        std::vector<ParseLineProp> getOrderParseLineProps();

        std::map<size_t, IScope *> getMatchedClass();

        HttpScope *getHttp();
};