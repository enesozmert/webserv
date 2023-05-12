#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include "../entity/Request.hpp"
#include "../utils/Utils.hpp"
#include "../function/VariableFunc.hpp"

class Request;
class ParserRequest
{
    private:
        Request *parsedRequest;
        Request request;
        std::string _text;
        std::vector<std::string> _lines;
        std::string _firstLine;
    public:
        ParserRequest();
        ~ParserRequest();
        ParserRequest(std::string text);

        void parse(std::string text);
        void parseFirstLine();
        void parseKeyValue();
        void parseBody();

        std::string  findKey(const std::string &line);
        std::string  findValue(const std::string &line);

        std::vector<std::string> getLines();
        std::string getText();
        void setText(std::string text);
        Request *getRequest();
};
