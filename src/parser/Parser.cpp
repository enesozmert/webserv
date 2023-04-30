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

void Parser::updateValueUsingAt(std::map<std::string, bool> &myMap, const std::string &key, bool newValue)
{
    try
    {
        myMap.at(key) = newValue;
    }
    catch (const std::out_of_range &oor)
    {
        std::cerr << "Key '" << key << "' not found in map." << std::endl;
    }
}

void Parser::parserScope(const std::vector<std::string> &lines)
{
    // http htpp
    // Server *server;
    // if (lines[i] == "server(")
    // {
    //     while(lines[i] == '}' && ';' yoksa)
    //     {
    //         server[i].server_name = lines[i][j]
    //         server[i].listen = lines[i][j + 1]
    //         if (lines[i] == "location")
    //         {
    //             server[i].location[k].allow = lines[i][j]
    //             server[i].location[k].root = lines[i][j + 1]
    //         }
    //     }
    // }

    // http.server = server;

    std::vector<std::string> scopeNames;
    for (size_t i = 0; i < lines.size(); i++)
    {
        std::string line = lines[i];
        std::string lineTrim = this->trim(line, " \n\t\r{}");
        if (lineTrim.find("http") != std::string::npos && lineTrim.length() == 4)
        {
            scopeNames.push_back(lineTrim);
        }
        else if (lineTrim.find("server") != std::string::npos && lineTrim.length() == 6)
        {
            scopeNames.push_back(lineTrim);
        }
        else if (line.find("location") != std::string::npos)
        {
            scopeNames.push_back(lineTrim.substr(0, 8));
        }
    }

    // for (size_t i = 0; i < scopeNames.size(); i++)
    // {
    //     std::cout << scopeNames[i] << std::endl;
    // }
    std::reverse(scopeNames.begin(), scopeNames.end());
    for (size_t i = lines.size(); i > 0; i--)
    {
        std::string line = lines[i];
        std::string lineTrim = this->trim(line, " \n\t\r{}");
        for (size_t j = 0; j < scopeNames.size(); j++)
        {
            if (lineTrim.find("location") != std::string::npos)
            {
                lineTrim = "location";
            }
            if (lineTrim == scopeNames[j] && lineTrim.find("{") != std::string::npos)
            {
                std::cout << line << std::endl;
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