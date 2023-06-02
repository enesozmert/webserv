#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <iterator>
#include "../entity/ParseLineProp.hpp"
#include "../error/ConfigException.hpp"

class SyntaxConfig
{
    private:
        ConfigException                 _configException;
        std::map<size_t, ParseLineProp> _parseLineProps;
        int                             _httpScopeCount;
        int                             _serverScopeCount;
    public:
        SyntaxConfig();
        ~SyntaxConfig();
        SyntaxConfig(const SyntaxConfig &syntaxConfig);
        SyntaxConfig(std::map<size_t, ParseLineProp> parseLineProps);
        SyntaxConfig &operator=(const SyntaxConfig &syntaxConfig);

        void    setParseLineProps(std::map<size_t, ParseLineProp> parseLineProps);
        void    analizer();
    private:
        int checkSemicolon(const int &index);
        int checkBrackets(const int &index);
        int checkVariableSpace(const int &index);
        int checkHttpCountGreaterThanZero(const int &index);
        int checkServerCountEqualZero(const int &index);
};