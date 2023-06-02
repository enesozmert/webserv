#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "../database/DataBase.hpp"
#include "../function/ContentTypeVariableFunc.hpp"

class HttpContentType
{
    private:
        DataBase<HttpVariable<std::string, std::string> >    _keywordDatabase;
    public:
        HttpContentType();
        ~HttpContentType();
        HttpContentType(const HttpContentType &httpContentType);

        bool            checkType(std::string type);
        HttpVariable<std::string, std::string> getByType(std::string type);
        std::string contentTypeGenerator(std::string type);
        DataBase<HttpVariable<std::string, std::string> > getKeywordDataBase();
        void    setKeywordDatabase(DataBase<HttpVariable<std::string, std::string> > keywordDatabase);
        void    keywordFill();
};
