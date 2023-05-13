Selected::RequestConfig(ConfigServer &config, Request &request, const std::string &path,  const std::string &method, std::string &locationName):
_error_page(config.getErrorPage()),
_client_body_buffer_size(config.getClientBodyBufferSize()),
_cgi_param(config.getCgiParam()),
_cgi_pass(config.getCgiPass()),
_allowed_methods(config.getAllowedMethods()),
_lang(""),
_autoindex(config.getAutoIndex())
{
	std::string	alias = config.getAlias();
	std::string	root = config.getRoot();
	std::string	ret;

	std::vector<std::string> conf_index = config.getIndex();
	for (std::vector<std::string>::const_iterator it = conf_index.begin();\
		it != conf_index.end(); it++) {

		std::vector<std::string>::const_iterator it2 = _index.begin();
		for (it2 = _index.begin();\
			it2 != _index.end(); it2++) {
				if (*it == *it2)
					break;
			}
		if (it2 == _index.end())
			_index.push_back(*it);
		}
	_cgi_param = request.getEnv();
	if (locationName[0] != '*' && config.getAliasSet()) {
		ret = root + alias + path.substr(locationName.length());
		this->_contentLocation = alias + removeAdjacentSlashes(path.substr(locationName.length()));
	}
	else {
		ret = root + path;
		this->_contentLocation = removeAdjacentSlashes(path);
	}
	this->_path = removeAdjacentSlashes(ret);
	std::string indexPath;
	if (!pathIsFile(this->_path) && method == "GET") {
		if ((indexPath = this->addIndex(request)) != "") {
			config = config.getLocationForRequest(indexPath, locationName);
			this->_cgi_pass = config.getCgiPass();
			this->_cgi_param = config.getCgiParam();
		}
	}
}

Selected	Selected::getConfigForRequest(Request &request)
{
    std::string     host;
    Server server;
    LocationScope location;

    host = request.getHeaders().at("Host");

	this->_hostname = host.substr(0, host.find_last_of(':'));
	server = this->getServerForRequest();
	location = location.getLocationForRequest(request.getPath());
	if (*(--this->locationPath.end()) == '/')
		this->locationPath.resize(this->locationPath.size() - 1);
    Selected config(server, request, uri, method, locationPath);
    config.setHostPort(address);
	return config;
}





LocationScope						Selected::getLocationForRequest(std::string const path) 
{
	std::string::size_type	tryLen = path.length();
	std::map<std::string, Server>::iterator	iter;
	std::string									tryLocation;

	if (!tryLen)
		return *this;
	if (!this->_location.empty()) 
    {
		do {
			tryLocation = path.substr(0, tryLen);
			iter = this->_location.find(tryLocation);
			if (iter != this->_location.end() && iter->first[0] != '*') {
				this->locationPath = tryLocation;
				return iter->second.getLocationForRequest(path);
			}
			tryLen--;
		} while (tryLen);
		for (std::map<std::string, Server>::iterator i = this->_location.begin(); i != this->_location.end(); i++)
        {
			if (i->first[0] == '*')
            {
				std::string	suffix = i->first.substr(1);
				if (path.length() > suffix.length() && !path.compare(path.length() - suffix.length(), suffix.length(), suffix))
                {
					return i->second.getLocationForRequest(path);
				}
			}
		}
	}
	return (*this);
}