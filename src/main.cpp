#include "../inc/parser/ParserConfig.hpp"

#include <iostream>
#include <string>

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Hata" << std::endl;
        return (-1);
    }
    std::string av1;
    ParserConfig *parser = new ParserConfig();
    HttpScope *http;

    av1 = av[1];
    parser->parse(av1);
    http = parser->getHttp();

    std::cout << "result1 : " << http->getServers().at(0)->getServerName().at(0) << std::endl;
    std::cout << "result2 : " << http->getServers().at(0)->getHost() << std::endl;
    std::cout << "result3 : " << http->getServers().at(1)->getLocations().at(0)->getRoot() << std::endl;
    // std::string httpRequest = "POST /processsampleform.php HTTp/1.1\r\n"
    //                           "Host: 128.2.210.139\r\n"
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
    return (0);
}