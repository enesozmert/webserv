#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "../database/DataBase.hpp"
#include "../function/StatusCodeVariableFunc.hpp"

class HttpStatusCode
{
    private:
        DataBase<HttpVariable<int, std::string> >    _keywordDatabase;
    public:
        HttpStatusCode();
        ~HttpStatusCode();
        HttpStatusCode(const HttpStatusCode &httpStatusCode);

        bool            checkStatusCode(int statusCode);
        HttpVariable<int, std::string> getByStatusCode(int statusCode);

        DataBase<HttpVariable<int, std::string> > getKeywordDataBase();
        void    setKeywordDatabase(DataBase<HttpVariable<int, std::string> > keywordDatabase);
        void    keywordFill();
};
