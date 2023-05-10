#pragma once

#include <vector>
#include <sstream>
#include <string>

inline std::string	to_string(size_t n)
{
	std::stringstream tmp;

	tmp << n;

	return tmp.str();
}

inline char	**mapToEnv(std::map<std::string, std::string> map)
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

inline std::string trim(const std::string &str, const std::string &delimiters)
{
    size_t first = str.find_first_not_of(delimiters);
    if (first == std::string::npos)
    {
        return "";
    }
    size_t last = str.find_last_not_of(delimiters);
    return str.substr(first, (last - first + 1));
}

inline std::string cleanString(std::string str)
{
    std::string cleanedStr;
    const char *ch = str.c_str();
    for (size_t i = 0; i < str.length(); i++)
    {
        if (ch[i] != '\t' || ch[i] != '\n')
        {
            cleanedStr += ch[i];
        }
    }
    return cleanedStr;
}