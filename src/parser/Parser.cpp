#include "../inc/parser/Parser.hpp"

typedef void (Parser::*method_function)(std::vector<size_t>);

Parser::Parser(/* args */) : _matchedClassIndex(0)
{
}

Parser::~Parser()
{
}

void Parser::parse(std::string &fileName)
{
    File file(fileName);
    size_t pos = 0;
    std::string fileContents = file.readToString();
    std::string fileCleanContents = cleanString(fileContents);
    std::vector<std::string> lines;
    std::string delimiter = "{};#";
    while ((pos = fileCleanContents.find_first_of(delimiter)) != std::string::npos)
    {
        std::string line = fileCleanContents.substr(0, pos + 1);
        std::string lineTrim = this->trim(line, " \r\t");
        lines.push_back(lineTrim);
        fileCleanContents.erase(0, pos + 1);
    }
    file.close();
    setScopeNames(lines);
    parseScope(lines);
    this->locationPtr = dynamic_cast<Location *>(_matchedClass.at(0));
    std::cout << "this->_matchedClass.at(0)1 : " << *this->locationPtr->getKeywordDataBase().getByNameData<IsVariableNameEqual>("auto_index").getValue() << std::endl;
    std::cout << "this->_matchedClass.at(0)2 : " << this->locationPtr->getAutoindex() << std::endl;

    this->serverPtr = dynamic_cast<Server *>(_matchedClass.at(2));
    std::cout << "this->_matchedClass.at(0)1 : " << *this->serverPtr->getKeywordDataBase().getByNameData<IsVariableNameEqual>("server_name").getValue() << std::endl;
    std::cout << "this->_matchedClass.at(0)2 : " << this->serverPtr->getServerName().at(1) << std::endl;
}

void Parser::parseScope(const std::vector<std::string> &lines)
{
    std::string line;
    std::string lineTrim;
    std::string lineTrimScope;
    std::string scopeName;
    size_t scopeOpenCount = 0;
    size_t scopeOpenIndex = 0;
    size_t scopeCloseIndex = 0;
    std::vector<std::string> tempScopes;

    for (size_t i = 0; i < lines.size(); i++)
    {
        ParseLineProp *parseLineProp = new ParseLineProp();

        line = lines[i];
        lineTrim = this->trim(line, " \n\t\r");
        lineTrimScope = this->trim(line, " \n\t\r{}");

        parseLineProp->setIndex(i);
        parseLineProp->setLine(lineTrim);
        if (lineTrim.find("location") != std::string::npos)
        {
            lineTrim = "location";
        }
        if (lineTrim.find(_scopeNames[scopeOpenCount]) != std::string::npos && line.find("{") != std::string::npos)
        {
            scopeOpenIndex = i;
            scopeName = _scopeNames[scopeOpenCount];
            parseLineProp->setIsScopeOpen(true);
            scopeCloseIndex = findClosingScopeIndex(lines, i);
        }
        if (line.find("{") != std::string::npos)
        {
            parseLineProp->setIsScopeOpen(true);
            scopeOpenIndex = i;
            scopeOpenCount++;
        }
        parseLineProp->setScopeOpenIndex(scopeOpenIndex);
        parseLineProp->setScopeName(scopeName);
        if (line.find("}") != std::string::npos)
        {
            parseLineProp->setIsScopeClose(true);
            scopeCloseIndex = i;
            parseLineProp->setScopeOpenIndex(findOpeningScopeIndex(lines, i));
            parseLineProp->setScopeName(_parseLineProps[findOpeningScopeIndex(lines, i)].getScopeName());
        }
        parseLineProp->setScopeCloseIndex(scopeCloseIndex);
        _parseLineProps.insert(std::pair<size_t, ParseLineProp>((parseLineProp)->getIndex(), *parseLineProp));
        delete parseLineProp;
    }
    setOrderParseLineProps();
    setOrderScopeNames();
    parseMatchClass();
    parseScopeFill();
}

void Parser::parseMatchClass()
{
    IScope *classes[3] = {&this->http, &this->server, &this->location};
    std::string method_function_name[3] = {"http", "server", "location"};

    for (size_t i = 0; i < _orderScopeNames.size(); i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            if (method_function_name[j] == _orderScopeNames[i])
            {
                IScope *class_instance = classes[j];
                _matchedClass.insert(std::make_pair(i, class_instance->clone()));
            }
        }
    }
}

void Parser::parseScopeFill()
{
    size_t tempCloseIndex;
    std::vector<size_t> tempLines;
    std::string method_function_name[3] = {"http", "server", "location"};
    method_function p[3] = {&Parser::parseHttp, &Parser::parseServer, &Parser::parseLocation};

    tempCloseIndex = _orderParseLineProps[0].getScopeCloseIndex();
    // for (size_t i = 0; i < _orderParseLineProps.size(); i++)
    // {
    //     std::cout << "parseLineProps[i].getIndex() : " << _orderParseLineProps[i].getIndex() << std::endl;
    //     std::cout << "parseLineProps[i].getLine() : " << _orderParseLineProps[i].getLine() << std::endl;
    //     std::cout << "parseLineProps[i].getScopeOpenIndex() : " << _orderParseLineProps[i].getScopeOpenIndex() << std::endl;
    //     std::cout << "parseLineProps[i].getScopeName() : " << _orderParseLineProps[i].getScopeName() << std::endl;
    //     std::cout << "parseLineProps[i].getScopeCloseIndex() : " << _orderParseLineProps[i].getScopeCloseIndex() << std::endl;
    //     std::cout << "parseLineProps[i].getIsScopeOpen() : " << _orderParseLineProps[i].getIsScopeOpen() << std::endl;
    //     std::cout << "parseLineProps[i].getIsScopeClose() : " << _orderParseLineProps[i].getIsScopeClose() << std::endl;
    //     std::cout << "****************************" << std::endl;
    // }
    for (size_t i = 0; i < _orderParseLineProps.size() + 1; i++)
    {
        if (tempCloseIndex != _orderParseLineProps[i].getScopeCloseIndex())
        {
            for (size_t j = 0; j < 3; j++)
            {
                if (_orderParseLineProps[i - 1].getScopeName() == method_function_name[j])
                {
                    (this->*p[j])(tempLines);
                    this->_matchedClassIndex++;
                    tempLines.clear();
                }
            }
            tempCloseIndex = _orderParseLineProps[i].getScopeCloseIndex();
        }
        if (tempCloseIndex == _orderParseLineProps[i].getScopeCloseIndex() && _orderParseLineProps[i].getIsScopeOpen() == false && _orderParseLineProps[i].getIsScopeClose() == false)
        {
            tempLines.push_back(_orderParseLineProps[i].getIndex());
        }
    }
}

void Parser::parseMatchedClassFill()
{
    
}

void Parser::parseHttp(std::vector<size_t> tempScopes)
{
    std::string tempVariableName;
    std::string tempVariableValue;

    this->httpPtr = dynamic_cast<Http *>(_matchedClass.at(_matchedClassIndex));
    if (this->httpPtr == NULL)
    {
        std::cout << "nulll" << _matchedClassIndex << std::endl;
    }
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::istringstream s(this->trim(this->_parseLineProps.at(tempScopes[i]).getLine(), ";"));
        s >> tempVariableName;
        std::getline(s >> std::ws, tempVariableValue);
        if (this->httpPtr->getKeywordDataBase().isHere<IsVariableNameEqual>(tempVariableName))
        {
            this->httpPtr->getKeywordDataBase().updateData<IsVariableNameEqual, std::string>(tempVariableName, tempVariableValue);
        }
        std::cout << "http ok : " << tempScopes[i] << std::endl;
    }
    std::cout << "**********parseHttp" << std::endl;
}

void Parser::parseServer(std::vector<size_t> tempScopes)
{
    std::string tempVariableName;
    std::string tempVariableValue;

    this->serverPtr = dynamic_cast<Server *>(_matchedClass.at(_matchedClassIndex));
    if (this->serverPtr == NULL)
    {
        std::cout << "nulll" << _matchedClassIndex << std::endl;
    }
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::istringstream s(this->trim(this->_parseLineProps.at(tempScopes[i]).getLine(), ";"));
        s >> tempVariableName;
        std::getline(s >> std::ws, tempVariableValue);
        if (this->serverPtr->getKeywordDataBase().isHere<IsVariableNameEqual>(tempVariableName))
        {
            this->serverPtr->getKeywordDataBase().updateData<IsVariableNameEqual, std::string>(tempVariableName, tempVariableValue);
        }
        std::cout << "Server ok : " << tempScopes[i] << std::endl;
    }
    std::cout << "**********parseServer" << std::endl;
}

void Parser::parseLocation(std::vector<size_t> tempScopes)
{
    std::string tempVariableName;
    std::string tempVariableValue;

    this->locationPtr = dynamic_cast<Location *>(_matchedClass.at(_matchedClassIndex));
    if (this->locationPtr == NULL)
    {
        std::cout << "nulll" << _matchedClassIndex << std::endl;
    }
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::istringstream s(this->trim(this->_parseLineProps.at(tempScopes[i]).getLine(), ";"));
        s >> tempVariableName;
        std::getline(s >> std::ws, tempVariableValue);
        if (this->locationPtr->getKeywordDataBase().isHere<IsVariableNameEqual>(tempVariableName))
        {
            this->locationPtr->getKeywordDataBase().updateData<IsVariableNameEqual, std::string>(tempVariableName, tempVariableValue);
        }
        std::cout << "locaation ok : " << tempScopes[i] << std::endl;
    }
    _matchedClass.at(_matchedClassIndex) = this->locationPtr;

    std::cout << "**********parseLocation" << std::endl;
}

size_t Parser::findClosingScopeIndex(const std::vector<std::string> &lines, size_t startingIndex)
{
    size_t openingScopesCount = 1;
    size_t closingScopesCount = 0;

    for (size_t i = startingIndex + 1; i < lines.size(); i++)
    {
        std::string line = lines[i];

        if (line.find("{") != std::string::npos)
        {
            openingScopesCount++;
        }
        if (line.find("}") != std::string::npos)
        {
            closingScopesCount++;
            if (openingScopesCount == closingScopesCount)
            {
                return i;
            }
        }
    }
    return 0;
}

size_t Parser::findOpeningScopeIndex(const std::vector<std::string> &lines, size_t closingIndex)
{
    size_t openingScopesCount = 0;
    size_t closingScopesCount = 1;

    for (size_t i = closingIndex - 1; i > 0; i--)
    {
        std::string line = lines[i];

        if (line.find("{") != std::string::npos)
        {
            openingScopesCount++;
            if (openingScopesCount == closingScopesCount)
            {
                return i;
            }
        }
        if (line.find("}") != std::string::npos)
        {
            closingScopesCount++;
        }
    }
    return 0;
}

void Parser::setScopeNames(const std::vector<std::string> &lines)
{

    for (size_t i = 0; i < lines.size(); i++)
    {
        std::string line = lines[i];
        std::string lineTrim = this->trim(line, " \n\t\r{}");
        if (lineTrim.find("http") != std::string::npos && lineTrim.length() == 4)
        {
            _scopeNames.push_back(lineTrim);
        }
        else if (lineTrim.find("server") != std::string::npos && lineTrim.length() == 6)
        {
            _scopeNames.push_back(lineTrim);
        }
        else if (line.find("location") != std::string::npos)
        {
            _scopeNames.push_back(lineTrim.substr(0, 8));
        }
    }
}

std::vector<std::string> Parser::getScopeNames()
{
    return (this->_scopeNames);
}

void Parser::setOrderScopeNames()
{
    ParseLineProp parseLineProp;

    parseLineProp = _orderParseLineProps[0];
    this->_orderScopeNames.push_back(_orderParseLineProps[0].getScopeName());
    for (size_t i = 1; i < _orderParseLineProps.size(); i++)
    {
        if (parseLineProp.getScopeCloseIndex() != _orderParseLineProps[i].getScopeCloseIndex())
        {
            this->_orderScopeNames.push_back(_orderParseLineProps[i].getScopeName());
        }
        parseLineProp = _orderParseLineProps[i];
    }
}

std::vector<std::string> Parser::getOrderScopeNames()
{
    return (this->_orderScopeNames);
}

std::map<size_t, ParseLineProp> Parser::getParseLineProps()
{
    return (this->_parseLineProps);
}

void Parser::setOrderParseLineProps()
{
    for (size_t i = 0; i < _parseLineProps.size(); i++)
    {
        _orderParseLineProps.push_back(_parseLineProps.at(i));
    }
    std::sort(_orderParseLineProps.begin(), _orderParseLineProps.end(), compareByScopeOpenIndex);
}

std::vector<ParseLineProp> Parser::getOrderParseLineProps()
{
    return (this->_orderParseLineProps);
}

std::map<size_t, IScope *> Parser::getMatchedClass()
{
    return (this->_matchedClass);
}

std::string Parser::trim(const std::string &str, const std::string &delimiters)
{
    size_t first = str.find_first_not_of(delimiters);
    if (first == std::string::npos)
    {
        return "";
    }
    size_t last = str.find_last_not_of(delimiters);
    return str.substr(first, (last - first + 1));
}

std::string Parser::cleanString(std::string str)
{
    std::string cleanedStr;
    const char *ch = str.c_str();
    for (size_t i = 0; i < str.length(); i++)
    {
        if (ch[i] != '\t' && ch[i] != '\n')
        {
            cleanedStr += ch[i];
        }
    }
    return cleanedStr;
}
