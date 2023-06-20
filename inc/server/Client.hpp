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

		size_t								ContentLen;
		Server								*sv;
		ParserRequest* 						_parser;
		Request*   							_request;
		HttpScope*  						_http;
		Response* 							response;
		int                         		locationIndex;
		std::string 						_response;
		unsigned int						_host;
		int									_port;
		std::string 						method;
		int									multi;
		int									status;
		int									isFav;
		std::string							body;

	public:
		Client(Server *sv, HttpScope* Hscope);
		/* Client();
		Client(const Client &client);
        Client &operator=(const Client &client);
		~Client(); */

		std::string		process(std::string multi);
	
		int				getStatus() const;
		int				getMulti() const;
		int				getIsFav() const;
		std::string		getMethod() const;
		size_t			getContentLen() const;
		std::string		getBody() const;
		void			getLocationForRequest(ServerScope *matchedServerScope, const std::string& path);
		ServerScope*	getServerForRequest();



		void	setParserRequest(std::string buffer);
		void	setRequest(Request *request);
		void	setMulti(int multi);
		void	setIsFav(int isFav);
		void	setStatus(int status);
		void	setMethod(std::string method);
		void	setContentLen(int ContentLen);
		void	setBody(std::string body);
};
