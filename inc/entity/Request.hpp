#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "../database/DataBase.hpp"
#include "IRequest.hpp"
#include "Variable.hpp"

class Request : public IRequest
{
    private:
    	std::string _version;
    	std::string _path;
    	std::string _body;
    	std::string	_query;
        std::string _env_for_cgi;
    	std::string _port;
        std::string	_languages;
        std::string _raw;
        int return_code;

        DataBase<Variable<std::string> > _keywordDatabase;
    public:
        Request();
        ~Request();
        Request(const Request& request);

        std::string getVersion();
        std::string getPath();
        std::string getBody();
        std::string getQuery();
        std::string getEnvForCgi();
        int         getPort();
        std::vector<std::pair<std::string, float> > getLanguages();
        std::string getRaw();

        std::string getName() const;
        Request* clone() const;
        Request* cloneNew() const;
};