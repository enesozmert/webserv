#pragma once

// Colors
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define	BLUE	"\033[34m"
#define	PURPLE	"\033[35m"
#define	CYAN	"\033[36m"
#define RESET	"\033[0m"

#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include "../entity/CgiVariable.hpp"

inline std::string	removeAdjacentSlashes(const std::string &str)
{
	std::string	ret;
	bool		lastIsSlash = false;

	for (std::string::size_type i = 0; i < str.length(); i++) {
		if (str[i] == '/') {
			if (!lastIsSlash)
				ret.push_back(str[i]);
			lastIsSlash = true;
		}
		else {
			lastIsSlash = false;
			ret.push_back(str[i]);
		}
	}
	return ret;
}


inline int		pathIsFile(const std::string& path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0 )
	{
		if (s.st_mode & S_IFDIR)
			return 0;
		else if (s.st_mode & S_IFREG)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

inline unsigned int	strToIp(std::string strIp) 
{
	size_t  sep = 0;
	unsigned int   n;
	unsigned char  m[4];
	size_t  start = 0;
	if (strIp == "localhost")
		strIp = "127.0.0.1";
	for (unsigned int i = 3 ; i != std::numeric_limits<u_int32_t>::max(); i--) {
		sep = strIp.find_first_of('.', sep);
		std::string str = strIp.substr(start, sep);
		n = atoi(str.c_str());
		m[i] = static_cast<unsigned char>(n);
		sep++;
		start = sep;
	}
	unsigned final = *(reinterpret_cast<unsigned int *>(m));
	return final;
}

inline char	**mapToEnvForm(std::vector<CgiVariable<std::string, std::string> > map)
{
	char	**str = new char*[map.size() + 1];
	int	j = 0;
	for (std::vector<CgiVariable<std::string, std::string> >::const_iterator i = map.begin(); i != map.end(); i++)
	{
		std::string	element = i->getKey() + "=" + i->getValue();
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

inline unsigned long stoul_cxx98(const std::string& str, size_t* idx = 0, int base = 10)
{
    std::istringstream iss(str);
    unsigned long result = 0;
    iss >> result;

    if (base != 10)
    {
        // İkinci parametrede belirtilen tabana göre dönüşüm yapma
        unsigned long multiplier = 1;
        unsigned long temp = result;

        result = 0;

        while (temp > 0)
        {
            unsigned long digit = temp % base;
            result += digit * multiplier;
            multiplier *= base;
            temp /= base;
        }
    }

    if (idx != 0)
    {
        *idx = iss.tellg();
        if (*idx == static_cast<size_t>(-1))
        {
            // Dönüşüm başarısız oldu
            *idx = 0;
        }
    }

    return result;
}


inline std::string getPwd()
{
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        exit(1);
    }
	return (std::string(cwd));
}

template <typename T>
std::string to_string(const T& value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

inline	std::string	openNread(std::string file_path)
{
	std::fstream fd(file_path.c_str());
	std::string tmp;
	std::string result;

	if (!fd.is_open())
	{
		result = "<!DOCTYPE html>\n<html><title>404</title><body>There was an error finding your file</body></html>\n";
		return result;
	}
	while (getline(fd, tmp))
		result += tmp + '\n';
	fd.close();
	return result;
}

inline	std::string setDate()
{
	char buffer[100];
	struct timeval tv;
	struct tm *tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buffer);
}

inline std::string	add_headers_favicon(std::string _body)
{
	std::string date = setDate();
	std::string headers;
	headers += "HTTP/1.1 200 OK\n";
	headers += "Date: " + date + "\n";
	headers += "Server: webserv\n";
	headers += "Content-Type: image/png\n";
	headers += "Content-Length: " + to_string(_body.size()) + "\n";
	headers += "Connection: close\n";
	std::string res;
	res = headers + "\n" + _body;
	return (res);
}
