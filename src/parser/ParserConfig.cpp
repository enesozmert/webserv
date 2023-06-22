#include "../inc/parser/ParserConfig.hpp"

typedef void (ParserConfig::*method_function)(std::vector<size_t>);

ParserConfig::ParserConfig() : _matchedClassIndex(0)
{
}

ParserConfig::~ParserConfig() {}

std::string ParserConfig::getCheckFileExtension(File &file, std::string &fileName)
{
    if (!file.checkFileExtension(fileName, "config"))
        this->_configException.run(104);
    return (file.readToString());
}

void ParserConfig::parse(std::string &fileName)
{
    parseFile(fileName);
    setScopeNames(this->_lines);
    parseScope(this->_lines);
    setOrderParseLineProps();
    setOrderScopeNames();
    parseMatchClass();
    parseScopeFill();
    parseMatchedClassFill();
}

void ParserConfig::parseSyntax(std::string &fileName)
{
    parseFile(fileName);
    setScopeNames(this->_lines);
    parseScope(this->_lines);
}

void ParserConfig::parseFile(std::string &fileName)
{
    File file(fileName);
    size_t pos = 0;
    std::string fileContents;
    std::string fileCleanContents;
    std::string delimiter = "{};#";

    fileContents = getCheckFileExtension(file, fileName);
    fileCleanContents = cleanString(fileContents);
    while ((pos = fileCleanContents.find_first_of(delimiter)) != std::string::npos)
    {
        std::string line = fileCleanContents.substr(0, pos + 1);
        std::string lineTrim = trim(line, " \r\t\n");
        if (line.find("{") != std::string::npos)
        {
            this->_lines.push_back(trim(lineTrim, " \n\r\t{"));
            this->_lines.push_back("{");
        }
        else if (line.find("}") != std::string::npos)
        {
            if (trim(lineTrim, " \n\r\t}")[0] != '\0')
                this->_lines.push_back(trim(lineTrim, " \n\r\t}"));
            this->_lines.push_back("}");
        }
        else
        {
            this->_lines.push_back(trim(lineTrim, " \n\r\t"));
        }
        fileCleanContents.erase(0, pos + 1);
    }
    file.close();
    if (this->_lines.size() == 0)
        this->_configException.run(118);
}

void ParserConfig::parseScope(const std::vector<std::string> &lines)
{
    std::string line;
    std::string lineTrim;
    std::string lineTrimScope;
    std::string scopeName;
    size_t scopeOpenCount = 0;
    size_t scopeOpenIndex = 0;
    size_t scopeCloseIndex = 0;

    _parseLineProps.clear();
    _parseLineProps.erase(0);
    for (size_t i = 0; i < lines.size(); i++)
    {
        ParseLineProp *parseLineProp = new ParseLineProp();

        line = lines[i];
        lineTrim = trim(line, " \n\t\r");
        lineTrimScope = trim(line, " \n\t\r{}");

        parseLineProp->setIndex(i);
        parseLineProp->setLine(lineTrim);
        if (lineTrim.find("location") != std::string::npos)
        {
            lineTrim = "location";
        }
        if (lineTrim.find(_scopeNames[scopeOpenCount]) != std::string::npos && lines[i + 1].find("{") != std::string::npos)
        {
            scopeOpenIndex = i;
            scopeName = _scopeNames[scopeOpenCount];
            parseLineProp->setIsScopeOpen(true);
            scopeCloseIndex = findClosingScopeIndex(lines, i + 1);
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
}

void ParserConfig::parseMatchClass()
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
                _matchedClass.insert(std::make_pair(i, class_instance->cloneNew()));
            }
        }
    }
}

void ParserConfig::parseScopeFill()
{
    size_t tempCloseIndex;
    std::vector<size_t> tempLines;
    std::string method_function_name[3] = {"http", "server", "location"};
    method_function p[3] = {&ParserConfig::parseHttp, &ParserConfig::parseServer, &ParserConfig::parseLocation};

    tempCloseIndex = _orderParseLineProps[0].getScopeCloseIndex();
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
        if (tempCloseIndex == _orderParseLineProps[i].getScopeCloseIndex() && 
            ((_orderParseLineProps[i].getIsScopeOpen() == false && _orderParseLineProps[i].getIsScopeClose() == false) || 
            (_orderParseLineProps[i].getScopeName() == "location" && _orderParseLineProps[i].getIsScopeOpen() == true)))
        {
            tempLines.push_back(_orderParseLineProps[i].getIndex());
        }
    }
}

void ParserConfig::parseMatchedClassFill()
{
    size_t serverIndex;

    serverIndex = 0;
    this->_parsedHttp = new HttpScope();
    for (size_t i = 0; i < _orderScopeNames.size(); i++)
    {
        if (_orderScopeNames[i].find("http") != std::string::npos)
        {
            this->httpPtr = dynamic_cast<HttpScope *>(_matchedClass.at(i));
        }
        else if (_orderScopeNames[i].find("server") != std::string::npos)
        {
            this->serverPtr = dynamic_cast<ServerScope *>(_matchedClass.at(i));
            this->_parsedHttp->setServer(this->serverPtr->clone());
        }
    }

    for (size_t i = 0; i < _orderScopeNames.size(); i++)
    {
        if (_orderScopeNames[i].find("location") != std::string::npos)
        {
            this->locationPtr = dynamic_cast<LocationScope *>(_matchedClass.at(i));
            this->_parsedHttp->getServers().at(serverIndex)->setLocation(this->locationPtr->clone());
        }
        if (_orderScopeNames[i].find("server") != std::string::npos)
        {
            serverIndex++;
        }
    }
}

void ParserConfig::parseHttp(std::vector<size_t> tempScopes)
{
    std::string tempVariableName;
    std::string tempVariableValue;

    this->httpPtr = dynamic_cast<HttpScope *>(_matchedClass.at(_matchedClassIndex));
    if (this->httpPtr == NULL)
    {
        this->_configException.run(115);
    }
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::istringstream s(trim(this->_parseLineProps.at(tempScopes[i]).getLine(), ";"));
        s >> tempVariableName;
        std::getline(s >> std::ws, tempVariableValue);
        if (this->httpPtr->getKeywordDataBase().isHere<std::string, IsVariableNameEqual>(tempVariableName))
        {
            this->httpPtr->getKeywordDataBase().updateData<IsVariableNameEqual, std::string>(tempVariableName, tempVariableValue);
        }
    }
}

void ParserConfig::parseServer(std::vector<size_t> tempScopes)
{
    std::string tempVariableName;
    std::string tempVariableValue;

    this->serverPtr = dynamic_cast<ServerScope *>(_matchedClass.at(_matchedClassIndex));
    if (this->serverPtr == NULL)
    {
        this->_configException.run(116);
    }
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::istringstream s(trim(this->_parseLineProps.at(tempScopes[i]).getLine(), ";"));
        s >> tempVariableName;
        std::getline(s >> std::ws, tempVariableValue);
        if (this->serverPtr->getKeywordDataBase().isHere<std::string, IsVariableNameEqual>(tempVariableName))
        {
            this->serverPtr->getKeywordDataBase().updateData<IsVariableNameEqual, std::string>(tempVariableName, tempVariableValue);
        }
    }
}

void ParserConfig::parseLocation(std::vector<size_t> tempScopes)
{
    std::string tempVariableName;
    std::string tempVariableValue;

    this->locationPtr = dynamic_cast<LocationScope *>(_matchedClass.at(_matchedClassIndex));
    if (this->locationPtr == NULL)
    {
        this->_configException.run(117);
    }
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::istringstream s(trim(this->_parseLineProps.at(tempScopes[i]).getLine(), ";"));
        s >> tempVariableName;
        std::getline(s >> std::ws, tempVariableValue);
        if (this->locationPtr->getKeywordDataBase().isHere<std::string, IsVariableNameEqual>(tempVariableName))
        {
            this->locationPtr->getKeywordDataBase().updateData<IsVariableNameEqual, std::string>(tempVariableName, tempVariableValue);
        }
    }
    _matchedClass.at(_matchedClassIndex) = this->locationPtr;
}

size_t ParserConfig::findClosingScopeIndex(const std::vector<std::string> &lines, size_t startingIndex)
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

size_t ParserConfig::findOpeningScopeIndex(const std::vector<std::string> &lines, size_t closingIndex)
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

void ParserConfig::setScopeNames(const std::vector<std::string> &lines)
{

    for (size_t i = 0; i < lines.size(); i++)
    {
        std::string line = lines[i];
        std::string lineTrim = trim(line, " \n\t\r{}");
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

std::vector<std::string> ParserConfig::getScopeNames()
{
    return (this->_scopeNames);
}

void ParserConfig::setOrderScopeNames()
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

std::vector<std::string> ParserConfig::getOrderScopeNames()
{
    return (this->_orderScopeNames);
}

std::map<size_t, ParseLineProp> ParserConfig::getParseLineProps()
{
    return (this->_parseLineProps);
}

void ParserConfig::setOrderParseLineProps()
{
    for (size_t i = 0; i < _parseLineProps.size(); i++)
    {
        _orderParseLineProps.push_back(_parseLineProps.at(i));
    }
    std::sort(_orderParseLineProps.begin(), _orderParseLineProps.end(), compareByScopeOpenIndex);
}

std::vector<ParseLineProp> ParserConfig::getOrderParseLineProps()
{
    return (this->_orderParseLineProps);
}

std::map<size_t, IScope *> ParserConfig::getMatchedClass()
{
    return (this->_matchedClass);
}

HttpScope *ParserConfig::getHttp()
{
    return (this->_parsedHttp);
}
