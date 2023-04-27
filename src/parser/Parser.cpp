#include "../inc/parser/Parser.hpp"

Parser::Parser(/* args */)
{
}

Parser::~Parser()
{
}

void Parser::printVec(std::vector<std::string> vec)
{
    std::vector<std::string>::iterator it = vec.begin();

    while(it != vec.end())
    {
        std::cout<<"I "<<*it<<" I ";
        it++;
    }
    std::cout<<std::endl;

}

Location Parser::locationParse(size_t i, std::vector<std::string> lines)
{
    std::vector<Location> locations;
    int location_index = 0;

    for (; i < lines.size(); i++)
    {
        std::string line = lines[i];
        std::cout<<"i "<<i<<" line :" << line << std::endl;
        
        //break;
        if (line.find("location") != std::string::npos)
        {
            //locations[location_index].setRoot();
//            std::cout<<"line :" << line << std::endl;
            std::cout<<"test"<<std::endl;
            break;
        }
       
    }
    return locations[location_index];
}

Server Parser::serverParse(size_t i, std::vector<std::string> lines)
{
    std::vector<Server> servers;
    int server_index = 0;

    for (; i < lines.size(); i++)
    {
        std::string line = lines[i];
        //std::cout<<"line :" << line << std::endl;
        
        //break;
        if (line.find("location") != std::string::npos)
        {
            i++;
            
            servers[server_index].setLocation(locationParse(i, lines));
            break;
        }
       
    }
    return servers[server_index];
}



void Parser::parse(std::string& fileName)
{
    File file(fileName);
    std::string fileContents = file.readToString();
    std::vector<std::string> lines;
    size_t pos = 0;
    //size_t i = 0;
    while ((pos = fileContents.find("server")) != std::string::npos)
    {
        pos = fileContents.find_first_of("\n", pos);
        std::string line = fileContents.substr(0, pos + 1);
        lines.push_back(line);
        fileContents.erase(0, pos + 1);

        //break;
    }
    Http http; 
    http.setServer(serverParse(1, lines));

    file.close();

}

