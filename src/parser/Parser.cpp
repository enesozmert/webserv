#include "../inc/parser/Parser.hpp"

Parser::Parser(/* args */)
{
}

Parser::~Parser()
{
}

void Parser::parse(std::string &fileName)
{
    File file(fileName);
    std::string fileContents = file.readToString();
    std::vector<std::string> lines;
    std::string delimiter = "{};";
    size_t pos = 0;
    while ((pos = fileContents.find_first_of(delimiter)) != std::string::npos)
    {
        std::string line = fileContents.substr(0, pos + 1);
        lines.push_back(line);
        fileContents.erase(0, pos + 1);
    }
    file.close();

    for (size_t i = 0; i < lines.size(); i++)
    {
        std::string line = lines[i];
        if (line.find("server_name") != std::string::npos)
        {
            std::cout << "Server name: " << line.substr(line.find("server_name") + 12) << std::endl;
        }
        // else if (line.find("listen") != std::string::npos)
        // {
        //     std::cout << "Port: " << line.substr(line.find("listen") + 7) << std::endl;
        // }
        // else if (line.find("root") != std::string::npos)
        // {
        //     std::cout << "Root directory: " << line.substr(line.find("root") + 6) << std::endl;
        // }
        // else if (line.find("index") != std::string::npos)
        // {
        //     std::cout << "Default file: " << line.substr(line.find("index") + 7) << std::endl;
        // }
    }
    // std::cout << fileContents << std::endl;
}