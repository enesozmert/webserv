#include <iostream>
#include <sstream>
#include <vector>
#include <string> 
#include "./inc/utils/Utils.hpp"
int main()
{
    std::string content = "------WebKitFormBoundaryAD3pnUzvTI5FByoh\nContent-Disposition: form-data; name=file; filename=images.png\r\nContent-Type:  image/png\r\n\r\nabababa";
    std::string _contentDisposition;
    std::vector<std::string> tokens;
    std::string token;
    //char delimiter = ';';
    //size_t i = content.find("\r\n\r\n");
    _contentDisposition = content.substr(content.find("Content-Disposition: "));
    std::cout << _contentDisposition << std::endl;
    /* std::stringstream ss(_contentDisposition);
   
    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(trim(token, " \r\n"));
    }

    for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
        std::cout << *it << std::endl;
    } */
    

    return 0;
}