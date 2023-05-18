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
    size_t i;
    size_t j;
    int result;

    i = 0;
    j = 0;
    method_function p[2] = {&SyntaxConfig::checkSemicolon, &SyntaxConfig::checkBrackets};
    for (size_t i = 0; i < this->_parseLineProps.size(); i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            result = (this->*p[j])(i);
            _configException.run(result);
        }
    }
}

int SyntaxConfig::checkSemicolon(const int &index)
{
    std::string line;
    std::string lineEndOf;

    line = this->_parseLineProps[index].getLine();
    if (!this->_parseLineProps[index].getIsScopeOpen() || !this->_parseLineProps[index].getIsScopeClose())
    {
        lineEndOf = line.substr(line.size() - 1, 1);
        std::cout << "lineEndOf : " << lineEndOf << std::endl;
        if (lineEndOf != ";")
        {
            std::cout << "line : " << line << std::endl;
            return (111);
        }
    }
    return (-1);
}
int SyntaxConfig::checkBrackets(const int &index)
{
    size_t scopeCloseIndex = 0;
    if (this->_parseLineProps[index].getLine().find("{") != std::string::npos)
    {
        scopeCloseIndex = this->_parseLineProps[index].getScopeCloseIndex();
    }
    if (this->_parseLineProps[scopeCloseIndex].getLine().find("}") == std::string::npos)
    {
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