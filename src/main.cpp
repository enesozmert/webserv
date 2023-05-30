#include "../inc/parser/ParserConfig.hpp"
#include "../inc/error/ConfigException.hpp"
#include "../inc/parser/ParserRequest.hpp"
#include "../inc/server/Cluster.hpp"
#include "../inc/syntax/SyntaxConfig.hpp"

#include <iostream>
#include <string>

void myTerminationHandler() {
    std::cerr << "Unhandled exception, program will terminate.\n";
    // abort();
}

int main(int ac, char **av)
{
   /*  (void)av;
    (void)ac;
     */
    std::string av1;
    //av1 = "configs/default.config";
    HttpScope *http;
    ParserConfig *parser = new ParserConfig();
    ParserConfig *parserSyntax = new ParserConfig();
    SyntaxConfig syntaxConfig;
    ConfigException configException;
    // std::set_terminate(myTerminationHandler);

   if (ac != 2)
    {
        configException.run(106);
        return (-1);
    }
    //get-filename
    av1 = av[1];

    //parseSyntaxForSyntaxAnalizer
    parserSyntax->parseSyntax(av1);
    syntaxConfig.setParseLineProps(parserSyntax->getParseLineProps());
    syntaxConfig.analizer();


    parser->parse(av1);
    http = parser->getHttp();
    /* std::cout << RED << "http->writeListens(http->getListens()) : " << RESET << std::endl;
    http->writeListens(http->getListens());
    // std::cout << "http->getServers().at(0)->getListen().host : " << http->getServers().at(0)->getListen().host << std::endl;
    // std::cout << "http->getServers().at(0)->getServerName().at(0) : " << http->getServers().at(0)->getServerName().at(0) << std::endl;
    // std::cout << "http->getServers().at(0)->getServerName().at(0) : " << http->getServers().at(0)->getLocations().at(0)->getIndex().at(1) << std::endl;
    // std::cout << "http->getServers().at(0)->getHost() : " << http->getServers().at(0)->getHost() << std::endl;
    // std::cout << "http->getServers().at(0)->getIp() : " << http->getServers().at(0)->getIp() << std::endl;
    // std::cout << "http->getServers().at(0)->getPort() : " << http->getServers().at(0)->getPort() << std::endl;
    // std::cout << "getPath() : " << http->getServers().at(0)->getLocations().at(1)->getPath() << std::endl;
    // std::cout << "http->getServers().at(0)->getHost() : " << http->getServers().at(0)->getHost() << std::endl;
    // std::cout << "http->getServers().at(1)->getLocations().at(0)->getRoot() : " << http->getServers().at(1)->getLocations().at(0)->getRoot() << std::endl;
    // std::cout << "http->getServers().at(1)->getLocations().at(0)->getRoot() : " << http->getServers().at(1)->getLocations().at(0)->getRoot() << std::endl;
    // std::cout << "cgi pass : " << http->getServers().at(0)->getLocations().at(0)->getCgiPass() << " end" << std::endl;
    // std::cout << "cgi pass : " << http->getServers().at(0)->getLocations().at(0)->getCgiParamKeyword() << " end" << std::endl;
    // std::cout << "cgi pass : " << http->getServers().at(0)->getLocations().at(0)->getCgiParamValues().at(0) << " end" << std::endl;
    // std::cout << "http->getServers().at(0)->getLocations().at(0)->getRedirectionCode() : " << http->getServers().at(0)->getLocations().at(0)->getRedirectionCode() << std::endl;
    // std::cout << "http->getServers().at(0)->getLocations().at(0)->getRedirectionValue() : " << http->getServers().at(0)->getLocations().at(0)->getRedirectionValue() << std::endl;
    // std::cout << "http->getServers().at(0)->getLocations().at(0)->getErrorPageCodes().at(0) : " << http->getServers().at(0)->getLocations().at(0)->getErrorPageCodes().at(0) << std::endl;
    // std::cout << "http->getServers().at(0)->getLocations().at(0)->getErrorPagePath() : " << http->getServers().at(0)->getLocations().at(0)->getErrorPagePath() << std::endl;
   
    // std::string httpRequest = "POST /processsampleform.php HTTP/1.1\r\n"
    //                           "Host: www.tutorialspoint.com\r\n"
    //                           "User-Agent: Mozilla/5.0 (windows; U; Windows NT 6.0; en-Us; rv:1.9.0.19) Gecko/2010031422 Firefox/3.0.19 (.NET CLR 3.5.30729)\r\n"
    //                           "Accept: text/html,application/xhtml+xm1,application/xm1;q=0.9,*/*;q=0.8\r\n"
    //                           "Accept-Language: en-us, en; q=0.5\r\n"
    //                           "Accept-Encoding: gzip, deflate\r\n"
    //                           "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
    //                           "Keep-Alive: 300\r\n"
    //                           "Connection: keep-alive\r\n"
    //                           "Referer: http://128.2.210.139/form. html\r\n"
    //                           "Content-Type: application/x-www-form-urlencoded\r\n"
    //                           "Content-Length: 28\r\n"
    //                           "\r\n"
    //                           "[Full request URI: http://128.2.210.139/processsampleform.php]";
    // ParserRequest *parserRequest = new ParserRequest();
    // Request *request;
    // parserRequest->parse(httpRequest);
    // request = parserRequest->getRequest();
    // std::cout << "request->getHost() : " << request->getHost() << std::endl;
    // std::cout << "request->getIp() : " << request->getIp() << std::endl;
    // std::cout << "request->getPort() : " << request->getPort() << std::endl;
    // std::cout << "request->getHttpMethodName() : " << request->getHttpMethodName() << std::endl;
    // std::cout << "request->getPath() : " << request->getPath() << std::endl;
    // std::cout << "request->getVersion() : " << request->getVersion() << std::endl;
    // std::cout << "request->getAcceptLanguages() : " << request->getAcceptLanguages().at(1).first << std::endl;

    Cluster cluster;
    cluster.setUpCluster(http);
    cluster.run();

    return (0);
}