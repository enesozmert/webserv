#include "../inc/parser/Parser.hpp"

#include <iostream>
#include <string>

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Hata" << std::endl;
        return (-1);
    }
    Parser *parser = new Parser();
    std::string av1 = av[1];
    parser->parse(av1);
    
    return (0);
}