#include "../inc/parser/Parser.hpp"

typedef void (Parser::*method_function)(std::vector<size_t>);

Parser::Parser(/* args */) : _matchedClassIndex(0)
{
}

Parser::~Parser()
{

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

std::vector<std::string> Parser::splitString(const std::string &str, const char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

void Parser::parseLocation(std::vector<size_t> tempScopes)
{
    // (void)tempScopes;
    std::string tempVariableName;
    std::string tempVariableValue;
    Variable<std::string>  findedVariable;
    DataBase<Variable<std::string> > database = this->location.getKeywordDataBase();
    DataBase<Variable<std::string> > databaseLocationPtr;

    std::cout << "_matchedClassIndex" << _matchedClassIndex << std::endl;
    this->locationPtr = dynamic_cast<Location *>(_matchedClass.at(_matchedClassIndex));
    if (this->locationPtr == NULL)
    {
        std::cout << "nulll" << _matchedClassIndex << std::endl;
    }
    databaseLocationPtr = locationPtr->getKeywordDataBase();
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::stringstream s(this->trim(this->_parseLineProps.at(tempScopes[i]).getLine(), ";"));
        s >> tempVariableName;
        s >> tempVariableValue;
        Variable<std::string> variableNew(tempVariableName, &tempVariableValue);
        if (database.isHere<IsVariableNameEqual>(tempVariableName))
        {
            findedVariable = this->locationPtr->getKeywordDataBase().getByNameData<IsVariableNameEqual>(tempVariableName);
            std::cout << "findedVariable.getName() " << findedVariable.getName() << std::endl;
            this->locationPtr->getKeywordDataBase().updateData<IsVariableNameEqual>(tempVariableName, variableNew);
            std::cout << "ok *********variable name : " << tempVariableName << " variable value :" << tempVariableValue << std::endl;
        }
        std::cout << "locaation ok : " << tempScopes[i] << std::endl;
    }

    std::cout << "**********parseLocation" << std::endl;
}

void Parser::parseServer(std::vector<size_t> tempScopes)
{
    // (void)tempScopes;
    // this->server.getKeywordDataBase();
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::cout << "Server ok : " << tempScopes[i] << std::endl;
    }
    std::cout << "**********parseServer" << std::endl;
}

void Parser::parseHttp(std::vector<size_t> tempScopes)
{
    // (void)tempScopes;
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::cout << "http ok : " << tempScopes[i] << std::endl;
    }
    std::cout << "**********parseHttp" << std::endl;
}

void Parser::parserLocationPath(const std::string &location)
{
    std::vector<std::string> locationPath = splitString(location, ' ');
    std::cout << "locationPath: " << locationPath[1] << std::endl;
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

    for (size_t i = 0; i < _orderScopeNames.size(); i++)
    {
        std::cout << "_orderScopeNames[i]" << _orderScopeNames[i] << std::endl;
    }

}

void Parser::setOrderParseLineProps()
{
    for (size_t i = 0; i < _parseLineProps.size(); i++)
    {
        _orderParseLineProps.push_back(_parseLineProps.at(i));
    }
    std::sort(_orderParseLineProps.begin(), _orderParseLineProps.end(), compareByScopeOpenIndex);
}

size_t Parser::getScopeOrderNameCount(std::vector<std::string> scopeNames, std::string scopeName)
{
    size_t scopeCount = 0;
    for (size_t i = 0; i < scopeNames.size(); i++)
    {
        if (scopeNames[i].find(scopeName) != std::string::npos)
        {
            scopeCount++;
        }
    }
    return (scopeCount);
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
    return 0; // not found
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
    return 0; // not found
}

void Parser::parseScopeFill()
{
    size_t tempCloseIndex;
    std::vector<size_t> tempLines;
    std::string method_function_name[3] = {"http", "server", "location"};
    method_function p[3] = {&Parser::parseHttp, &Parser::parseServer, &Parser::parseLocation};

    tempCloseIndex = _orderParseLineProps[0].getScopeCloseIndex(); // 8
    for (size_t i = 0; i < _orderParseLineProps.size(); i++)
    {
        std::cout << "parseLineProps[i].getIndex() : " << _orderParseLineProps[i].getIndex() << std::endl;
        std::cout << "parseLineProps[i].getLine() : " << _orderParseLineProps[i].getLine() << std::endl;
        std::cout << "parseLineProps[i].getScopeOpenIndex() : " << _orderParseLineProps[i].getScopeOpenIndex() << std::endl;
        std::cout << "parseLineProps[i].getScopeName() : " << _orderParseLineProps[i].getScopeName() << std::endl;
        std::cout << "parseLineProps[i].getScopeCloseIndex() : " << _orderParseLineProps[i].getScopeCloseIndex() << std::endl;
        // std::cout << "parseLineProps[i].getLineNotScope() : " << parseLineProps[i].getLineNotScope() << std::endl;
        std::cout << "parseLineProps[i].getIsScopeOpen() : " << _orderParseLineProps[i].getIsScopeOpen() << std::endl;
        std::cout << "parseLineProps[i].getIsScopeClose() : " << _orderParseLineProps[i].getIsScopeClose() << std::endl;
        std::cout << "****************************" << std::endl;
    }
    
    for (size_t i = 0; i < _orderParseLineProps.size() + 1; i++)
    {
        // std::cout << " _orderParseLineProps[i].getScopeCloseIndex() : " << _orderParseLineProps[i].getScopeCloseIndex() << std::endl;
        // std::cout << "tempCloseIndex = _orderParseLineProps[i].getScopeCloseIndex();" << tempCloseIndex << std::endl;
        if (tempCloseIndex != _orderParseLineProps[i].getScopeCloseIndex())
        {
            // std::cout << "eşit değil" << _orderParseLineProps[i].getIndex() << std::endl;
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
            // std::cout << "eşit" << _orderParseLineProps[i].getIndex() << std::endl;
            tempLines.push_back(_orderParseLineProps[i].getIndex());
        }
    }
}

void Parser::parseMatchedClassFill(std::map<size_t, IScope *> matchedClass)
{

    (void)matchedClass;
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
                // std::cout << "method_function_name[j] " << method_function_name[j] << std::endl;
                IScope *class_instance = classes[j];
                _matchedClass.insert(std::make_pair(i, class_instance->clone()));
            }
        }
    }
    // std::cout << matchedClass.size() << std::endl;
    // Location *location = dynamic_cast<Location*>(_matchedClass.at(2));
    // location->setPath("enes");
    // std::cout << "matchedClass.at(0)->getName() : " << location->getPath() << std::endl;
    // parseMatchedClassFill(_matchedClass);
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
        // parseLineProp->setLineNotScope(lineTrimScope);
        if (lineTrim.find("location") != std::string::npos)
        {
            lineTrim = "location";
        }
        if (lineTrim.find(_scopeNames[scopeOpenCount]) != std::string::npos && line.find("{") != std::string::npos)
        {
            scopeOpenIndex = i;
            scopeName = _scopeNames[scopeOpenCount];
            // parseLineProp->setIsScopeOpen(true);
            // std::cout << "is open true" << std::endl;
            // scopeOrderNames.erase(scopeOrderNames.begin() + j);
            // parseLineProp->setScopeCloseIndex(findClosingScopeIndex(lines, i));
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
            // size_t open = ;
            // parseLineProp->setScopeOpenIndex(parseLineProps[(parseLineProp)->getScopeCloseIndex()].getIndex());
            parseLineProp->setIsScopeClose(true);
            // parseLineProp->setScopeCloseIndex(i);
            scopeCloseIndex = i;
            parseLineProp->setScopeOpenIndex(findOpeningScopeIndex(lines, i));
            parseLineProp->setScopeName(_parseLineProps[findOpeningScopeIndex(lines, i)].getScopeName());
            // parseLineProp->setIsScopeClose(true);
            // std::cout << "Opening" << findOpeningScopeIndex(lines, i) << std::endl;
            // continue;
        }
        parseLineProp->setScopeCloseIndex(scopeCloseIndex);
        // ParseLineProp temp = *parseLineProp;
        // parseLineProp->setScopeOpenIndex(scopeOpenIndex);
        _parseLineProps.insert(std::pair<size_t, ParseLineProp>((parseLineProp)->getIndex(), *parseLineProp));
        delete parseLineProp;
    }
    // std::sort(parseLineProps.begin(), parseLineProps.end(), compareByScopeCloseIndex);
    // for (size_t i = 0; i < _parseLineProps.size(); i++)
    // {
    //     std::cout << "parseLineProps[i].getIndex() : " << _parseLineProps[i].getIndex() << std::endl;
    //     std::cout << "parseLineProps[i].getLine() : " << _parseLineProps[i].getLine() << std::endl;
    //     std::cout << "parseLineProps[i].getScopeOpenIndex() : " << _parseLineProps[i].getScopeOpenIndex() << std::endl;
    //     std::cout << "parseLineProps[i].getScopeName() : " << _parseLineProps[i].getScopeName() << std::endl;
    //     std::cout << "parseLineProps[i].getScopeCloseIndex() : " << _parseLineProps[i].getScopeCloseIndex() << std::endl;
    //     // std::cout << "parseLineProps[i].getLineNotScope() : " << parseLineProps[i].getLineNotScope() << std::endl;
    //     std::cout << "parseLineProps[i].getIsScopeOpen() : " << _parseLineProps[i].getIsScopeOpen() << std::endl;
    //     std::cout << "parseLineProps[i].getIsScopeClose() : " << _parseLineProps[i].getIsScopeClose() << std::endl;
    //     std::cout << "****************************" << std::endl;
    // }
    // std::cout << "ok*************************************" << std::endl;
    setOrderParseLineProps();
    setOrderScopeNames();
    parseMatchClass();
    parseScopeFill();
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
        // std::cout << "line : " << lineTrim << std::endl;
        fileCleanContents.erase(0, pos + 1);
    }
    file.close();
    setScopeNames(lines);
    parseScope(lines);
    // this->locationPtr = dynamic_cast<Location *>(_matchedClass.at(0));
    // std::cout << "this->_matchedClass.at(0) : " << this->locationPtr->getAutoindex() << std::endl;

    // std::cout << "lineCount" << lineCount << std::endl;
}
