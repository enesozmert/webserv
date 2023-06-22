#include "../inc/syntax/SyntaxConfig.hpp"

typedef int (SyntaxConfig::*method_function)(const int &);

SyntaxConfig::SyntaxConfig() : _httpScopeCount(0), _serverScopeCount(0) {}

SyntaxConfig::~SyntaxConfig() {}

SyntaxConfig::SyntaxConfig(const SyntaxConfig &syntaxConfig) : _httpScopeCount(0), _serverScopeCount(0)
{
    *this = syntaxConfig;
}

SyntaxConfig::SyntaxConfig(std::map<size_t, ParseLineProp> parseLineProps) : _httpScopeCount(0), _serverScopeCount(0)
{
    this->_parseLineProps = parseLineProps;
}

SyntaxConfig &SyntaxConfig::operator=(const SyntaxConfig &syntaxConfig)
{
    if (this == &syntaxConfig)
        return (*this);
    this->_parseLineProps = syntaxConfig._parseLineProps;
    return (*this);
}

void SyntaxConfig::setParseLineProps(std::map<size_t, ParseLineProp> parseLineProps)
{
    this->_parseLineProps = parseLineProps;
}

void SyntaxConfig::analizer()
{
    int result;

    result = -1;
    method_function p[4] = {&SyntaxConfig::checkSemicolon, &SyntaxConfig::checkBrackets,
                            &SyntaxConfig::checkHttpCountGreaterThanZero, &SyntaxConfig::checkVariableSpace};
    for (size_t i = 0; i < this->_parseLineProps.size(); i++)
    {
        result = (this->*p[0])(i);
        _configException.run(result);

        result = (this->*p[1])(i);
        _configException.run(result);

        result = (this->*p[2])(i);
        _configException.run(result);

        result = (this->*p[3])(i);
        _configException.run(result);
    }
}

int SyntaxConfig::checkSemicolon(const int &index)
{
    std::string line;

    if (!this->_parseLineProps[index].getIsScopeOpen() && !this->_parseLineProps[index].getIsScopeClose())
    {
        line = this->_parseLineProps[index].getLine();
        size_t newlinePos = line.find('\n');
        std::string newLineSubString = line.substr(0, newlinePos);
        size_t semicolonPos = newLineSubString.find(';');
        if (semicolonPos == std::string::npos || line[line.length() - 1] != ';')
        {
            return (111);
        }
    }
    return (-1);
}
int SyntaxConfig::checkBrackets(const int &index)
{
    size_t scopeCloseIndex = 0;
    if (!this->_parseLineProps[index].getIsScopeOpen() && !this->_parseLineProps[index].getIsScopeOpen())
        return (-1);
    if (this->_parseLineProps[index].getIsScopeOpen())
    {
        scopeCloseIndex = this->_parseLineProps[index].getScopeCloseIndex();
    }
    if (this->_parseLineProps[scopeCloseIndex].getLine() != "}")
    {
        std::cout << "this->_parseLineProps[scopeCloseIndex].getLine()" << this->_parseLineProps[index].getLine() << std::endl;
        return (108);
    }
    return (-1);
}
int SyntaxConfig::checkVariableSpace(const int &index)
{
    std::string line;
    bool found = false;

    if (!this->_parseLineProps[index].getIsScopeOpen() && !this->_parseLineProps[index].getIsScopeClose())
    {
        line = this->_parseLineProps[index].getLine();
        if (line[0] <= 32)
            return (122);
        if (line[line.length() - 2] <= 32)
            return (120);
        if (line[line.length() - 1] <= 32 && line[line.length() - 1] != '\0')
            return (121);
        for (size_t i = 0; i < line.length(); ++i)
        {
            if (line[i] <= 32)
            {
                if (found)
                {
                    return (119);
                }
                found = true;
            }
            else
            {
                found = false;
            }
        }
    }
    return (-1);
}
int SyntaxConfig::checkHttpCountGreaterThanZero(const int &index)
{
    int result = -1;
    if (this->_parseLineProps[index].getLine() == "http" && this->_parseLineProps[index].getScopeName() == "http" && this->_parseLineProps[index].getIsScopeOpen())
    {
        this->_httpScopeCount += 1;
    }
    this->_httpScopeCount != 1 ? result = 113 : result = -1;
    return (result);
}
int SyntaxConfig::checkServerCountEqualZero(const int &index)
{
    int result = -1;
    if (this->_parseLineProps[index].getLine() == "server" && this->_parseLineProps[index].getScopeName() == "server" && this->_parseLineProps[index].getIsScopeOpen())
    {
        this->_serverScopeCount = 1;
    }
    else
        return (-1);
    this->_serverScopeCount == 0 ? result = 114 : result = -1;
    return result;
}