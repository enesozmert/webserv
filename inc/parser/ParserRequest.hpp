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
        Request*                    parsedRequest;
        Request                     request;
        std::string                 _raw;
        std::vector<std::string>    _lines;
        std::string                 _firstLine;
        int                         status;
        int                         _multi;
    public:
        ParserRequest();
        ~ParserRequest();

        int     parse(std::string raw = "");
        void    parseFirstLine();
        void    parseKeyValue();
        void    parseBody();
        void    parseRequestClone();

        std::string  findKey(const std::string &line);
        std::string  findValue(const std::string &line);

        std::vector<std::string>    getLines();
        std::string                 getRaw();
        Request*                    getRequest();
        int	                        getStatus() const;
        int	                        getMulti() const;
    
        void                        setRaw(std::string raw);
        void                        writeRequest();
};
