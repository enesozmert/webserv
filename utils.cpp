#include "webserv.hpp"

std::string	to_string(size_t n)
{
	std::stringstream tmp;

	tmp << n;

	return tmp.str();
}

char	**maptoStr(std::map<std::string, std::string> map)
{
	char	**str = new char*[map.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = map.begin(); i != map.end(); i++)
	{
		std::string	element = i->first + "=" + i->second;
		str[j] = new char[element.size() + 1];
		str[j] = strcpy(str[j], (const char*)element.c_str());
		j++;
	}
	str[j] = NULL;
	return str;
}

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \n\r\t\f\v");
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}