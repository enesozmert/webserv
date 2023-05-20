#include "../inc/syntax/SyntaxConfig.hpp"

typedef int (SyntaxConfig::*method_function)(const int &);

SyntaxConfig::SyntaxConfig(/* args */)
{
}

SyntaxConfig::~SyntaxConfig()
{
}


SyntaxConfig::SyntaxConfig(const SyntaxConfig &syntaxConfig)
{
    *this = syntaxConfig;
}

SyntaxConfig::SyntaxConfig(std::map<size_t, ParseLineProp> parseLineProps)
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
    method_function p[2] = {&SyntaxConfig::checkSemicolon, &SyntaxConfig::checkBrackets};
    for (size_t i = 0; i < this->_parseLineProps.size(); i++)
    {
        result = (this->*p[0])(i);
        _configException.run(result);

        result = (this->*p[1])(i);
        _configException.run(result);
    }
}

int SyntaxConfig::checkSemicolon(const int &index)
{
    std::string line;

    line = this->_parseLineProps[index].getLine();
    if (line.find("{") == std::string::npos && line.find("}") == std::string::npos)
    {
        size_t newlinePos = line.find('\n');
        std::string newLineSubString = line.substr(0, newlinePos);
        size_t semicolonPos = newLineSubString.find(';');
        if (semicolonPos == std::string::npos)
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
    (void)index;
    return (false);
}
int SyntaxConfig::checkHttpCountGreaterThanZero(const int &index)
{
    (void)index;
    return (false);
}
int SyntaxConfig::checkServerCountGreaterThanZero(const int &index)
{
    (void)index;
    return (false);
}