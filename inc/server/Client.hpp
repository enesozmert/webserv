#pragma once

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <map>


#include "Server.hpp"
#include "../parser/ParserRequest.hpp"
#include "../entity/Request.hpp"
#include "../entity/Response.hpp"
#include "../function/ServerFunc.hpp"
#include "../entity/HttpScope.hpp"

class Client
{
	private:
		int									multi;
		int									status;
		size_t								postLen;
		Server								*sv;
		ParserRequest* 						_parser;
		Request*   							_request;
		HttpScope*  						_http;
		Response* 							response;
		int                         		locationIndex;
		std::string 						_response;
		unsigned int						_host;
		int									_port;
		int									_isFav;

	public:
		std::string							postVal;
		Client();
		Client(Server *sv);
		Client(const Client &client);
        Client &operator=(const Client &client);
		~Client();

		void			setParserRequest(std::string buffer);
		void			setRequest(Request *request);
		std::string		process();
		int				getStatus() const;
		int				getMulti() const;
		std::string		getPostVal() const;
		size_t			getPostLen() const;
		void			getLocationForRequest(ServerScope *matchedServerScope, const std::string& path);
		ServerScope*	getServerForRequest();
		int				getIsFav() const;
};
