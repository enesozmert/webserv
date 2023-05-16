#pragma once
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "../utils/Utils.hpp"

class Request
{
    private:
    	std::vector<std::string>					_methods;
    	std::string 								_method;
    	std::string									_path;
    	std::string 								_version;
    	std::string 								_body;
    	std::string									_query;
    	int											_port;
    	std::map<std::string, std::string> 			_headers;
    	std::map<std::string, std::string>			_env_for_cgi;
    	std::vector<std::pair<std::string, float> >	_languages;
    	const std::string&							_raw;
    	int 										return_code;

    public:
    	Request(const std::string& request_str);
    	~Request();

    	//getter
    	const std::map<std::string, std::string>&			getHeaders() const;
    	const std::map<std::string, std::string>&			getEnvforCGI() const;
    	const std::string&									getMethod() const;
    	const std::string&									getVersion() const;
    	int													getRet() const;
    	const std::string&									getBody() const;
    	int													getPort() const;
    	const std::string&									getPath() const;
    	const std::string&									getQuery() const;
    	const std::string&									getRaw() const;
    	const std::vector<std::pair<std::string, float> >&	getLanguage() const;

    	//setter
    	void	setReturnCode(int return_code);
    	void	setMethod(const std::string &method);
    	void	setLanguage();
    	void	setBody(const std::string& str);

    	void						initMethods();
    	void						findQuery();
    	std::string					convertHeadertoCGIformat(std::string& key);
    	std::string					createKey(const std::string& line);
    	std::string					createValue(const std::string& line);
    	int							checkVersion();
    	int							checkMethod();
    	void						parseFirstline(const std::string& str);
    	std::string					GetnextLine(const std::string &str, size_t& i);
    	int							parse(const std::string& str);
    	void						resetHeaders();

};

std::ostream&	operator<<(std::ostream& os, const Request& re);