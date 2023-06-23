#include "../inc/parser/ParserConfig.hpp"
#include "../inc/error/ConfigException.hpp"
#include "../inc/parser/ParserRequest.hpp"
#include "../inc/server/Cluster.hpp"
#include "../inc/syntax/SyntaxConfig.hpp"
#include "../inc/utils/Utils.hpp"

Cluster *clusterEnd;

void signalHandler(int signum)
{
    if (signum == SIGINT)
    {
        std::cout << "\nInterrupt signal (" << signum << ") received.\n";
        clusterEnd->cleanAll();
        std::cout << "\nSuccess.\n";
        exit(1);
    }
}

int main(int ac, char **av)
{

    std::string av1;
    HttpScope *http;
    ParserConfig *parser = new ParserConfig();
    ParserConfig *parserSyntax = new ParserConfig();
    SyntaxConfig syntaxConfig;
    ConfigException configException;

    if (ac != 2)
    {
        configException.run(106);
        return (-1);
    }
    av1 = av[1];
 
    parserSyntax->parseSyntax(av1);
    syntaxConfig.setParseLineProps(parserSyntax->getParseLineProps());
    syntaxConfig.analizer();

    parser->parse(av1);
    http = parser->getHttp();
    
    Cluster cluster;

    clusterEnd = &cluster;
    try
    {
        if (cluster.setUpCluster(http) == -1)
            return (-1);
        std::signal(SIGINT, signalHandler);
        cluster.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}