#include "../inc/parser/Parser.hpp"

Parser::Parser(/* args */)
{
}

Parser::~Parser()
{
}

std::string Parser::cleanString(std::string str) {
    std::string cleanedStr;
    const char *ch = str.c_str();
    for (size_t i = 0; i < str.length(); i++) {
        if (ch[i] != '\t' && ch[i] != '\n') {
            cleanedStr += ch[i];
        }
    }
    return cleanedStr;
}

std::string Parser::trim(const std::string& str, const std::string& delimiters) {
    size_t first = str.find_first_not_of(delimiters);
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(delimiters);
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> Parser::splitString(const std::string& str, const char delimiter)
{
    std::vector<std::string> tokens;
    std::string word;

    std::stringstream ss(str);
    while (!ss.eof()) {
        getline(ss, word, delimiter);
        tokens.push_back(word);
    }
    return tokens;
}

// void Parser::parseLocation(std::vector<std::string> locations)
// {

// }

void Parser::parserLocationPath(const std::string &location)
{
    std::cout << "location: " << location << std::endl;
    std::string lineTrim = this->trim(location, "\t{");
    std::vector<std::string> path = this->splitString(location, ' ');
    std::cout << "path : " << path[1] << std::endl;
}


void Parser::parse(std::string &fileName)
{
    File file(fileName);
    size_t pos = 0;
    std::string fileContents = file.readToString();
    std::string fileCleanContents = cleanString(fileContents);
    std::vector<std::string> lines;
    std::string delimiter = "{};";
    std::vector<std::string> locations;
    while ((pos = fileCleanContents.find_first_of(delimiter)) != std::string::npos)
    {
        std::string line = fileCleanContents.substr(0, pos + 1);
        lines.push_back(line);
        fileCleanContents.erase(0, pos + 1);
    }
    file.close();

    int inServerBlock = 0;
    for (size_t i = 0; i < lines.size(); i++)
    {
        std::string line = lines[i];
        if (line.find("{") != std::string::npos)
        {
            this->parserLocationPath(line);
            std::string lineTrim = this->trim(line, " {");
            std::cout << "line : " << line << std::endl;
            if (lineTrim.find("location") != std::string::npos)
            {
                inServerBlock++;
                continue;
            }
        }
        if (line.find("}") != std::string::npos && inServerBlock)
        {
            inServerBlock--;
            continue;
        }
        if (inServerBlock)
        {
            // server bloğundayız
            locations.push_back(line);
            std::cout << "Server ok: " << line << std::endl;
            // burada yapılacak işlemler
        }
        else
        {
            std::cout << "Server no" << std::endl;
            // server bloğunda değiliz
            // burada yapılacak işlemler
        }
    }
    // std::cout << fileContents << std::endl;
}