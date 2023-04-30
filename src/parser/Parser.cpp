#include "../inc/parser/Parser.hpp"

typedef void (Parser::*method_function)(std::vector<std::string>);

Parser::Parser(/* args */)
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

void Parser::location(std::vector<std::string> tempScopes)
{
    // (void)tempScopes;
    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::cout << "locaation ok : " << tempScopes[i] << std::endl;
    }

    std::cout << "**********parseLocation" << std::endl;
}

void Parser::server(std::vector<std::string> tempScopes)
{
    // (void)tempScopes;

    for (size_t i = 0; i < tempScopes.size(); i++)
    {
        std::cout << "Server ok : " << tempScopes[i] << std::endl;
    }
    std::cout << "**********parseServer" << std::endl;
}

void Parser::http(std::vector<std::string> tempScopes)
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

void Parser::parserScope(const std::vector<std::string> &lines)
{
    std::string scopeTexts[3];
    int inServerBlockOpen = 0;
    int inServerBlockClose = 0;

    scopeTexts[0] = "http";
    scopeTexts[1] = "server";
    scopeTexts[2] = "location";
    method_function p[3] = {&Parser::http, &Parser::server, &Parser::location};
    std::vector<std::string> tmpScopes;

    for (size_t j = 0; j < 3; j++)
    {
        for (size_t i = 0; i < lines.size(); i++)
        {
            std::string line = lines[i];
            std::string lineTrim = this->trim(line, " {");
            std::string lineTrimScope = this->trim(line, "{}");
            std::string lineTrimSpace = this->trim(line, " ");

            if (line.find(scopeTexts[j]) != std::string::npos)
            {
                if (scopeTexts[j] == "location")
                    this->parserLocationPath(lineTrim);
                if (line.find("{") != std::string::npos)
                {
                    inServerBlockOpen++;
                    continue;
                }
                // std::cout << "inServerBlock" << inServerBlock << std::endl;
            }
            if (line.find("{") != std::string::npos)
                {
                    inServerBlockOpen++;
                    continue;
                }
            if (line.find("}") != std::string::npos)
            {
                // inServerBlock--;
                inServerBlockClose++;
                continue;
            }

            if (inServerBlockOpen != inServerBlockClose)
            {
                // std::cout << "ok 1" << scopeTexts[j] << std::endl;
                tmpScopes.push_back(lineTrimScope);
            }
            else if (inServerBlockOpen == inServerBlockClose)
            {
                (this->*p[j])(tmpScopes);
                inServerBlockClose = 0;
                inServerBlockOpen = 0;
                tmpScopes.clear();
                // tmpScopes.resize(0);
                // break;
            }
        }
    }
}

void Parser::parse(std::string &fileName)
{
    File file(fileName);
    size_t pos = 0;
    std::string fileContents = file.readToString();
    std::string fileCleanContents = cleanString(fileContents);
    std::vector<std::string> lines;
    std::string delimiter = "{};";
    while ((pos = fileCleanContents.find_first_of(delimiter)) != std::string::npos)
    {
        std::string line = fileCleanContents.substr(0, pos + 1);
        lines.push_back(line);
        fileCleanContents.erase(0, pos + 1);
    }
    parserScope(lines);
    file.close();

    // std::cout << "lineCount" << lineCount << std::endl;
}