#include <iostream>
#include <string>
#include <sstream>

void	setBody(const std::string& str)
{
	std::string body;
    body.assign(str);
	while (body[body.size() - 1] == '\n' || body[body.size() - 1] == '\r')
	{
		if (body.size() > 0)
			body.resize(body.size() - 1);
	}
    std::cout << "body: " << body << std::endl;
}

 std::string getnext(std::string& str, size_t& i)
{
    size_t j = 0;
    std::string		subStr;

    while(j < str.size() - 1)
    {
        j = str.find_first_of('\n', i);
        subStr = str.substr(i, j - i);
        if (subStr[subStr.size() - 1] == '\r')
        {
            if (subStr.size())
                subStr.resize(subStr.size() - 1);
        }
        std::cout << "subStr: " << subStr << std::endl;
        std::cout << "subStr size: " << subStr.size() << std::endl;
        if (j == std::string::npos)
            i = j;
        i = j + 1;
    }
    return subStr;

} 


int main()
{
    size_t i = 0;
    std::string		line;
    std::string		str = "fatma\r\nozturk\r\n\r\nbody\r\n";
    while ((line = getnext(str, i)) != "\r" && line != "" )
    {
        std::cout << "while" << std::endl;
        std::cout << "line: " << line << std::endl;
        std::cout << "line size: " << line.size() << std::endl;
    }
    setBody(str.substr(i, std::string::npos));

    /* std::string    _method;
    std::string    _path;
    std::string    _version;

    std::stringstream s("POST sample.php HTTP/1.1");
	s >> _method;
    s >> _path;
    s >> _version;

    std::cout << "method: " << _method << std::endl;
    std::cout << "method size: " << _method.size() << std::endl;
    std::cout << "path: " << _path << std::endl;
    std::cout << "path size: " << _path.size() << std::endl;
    std::cout << "version: " << _version << std::endl;
    std::cout << "version size: " << _version.size() << std::endl; */

    return 0;
}