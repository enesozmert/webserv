#include "../inc/parser/ParserRequest.hpp"

ParserRequest::ParserRequest(/* args */)
{
}

ParserRequest::~ParserRequest()
{
}

ParserRequest::ParserRequest(std::string raw) : _raw(raw){}

void ParserRequest::parse(std::string raw)
{
    std::string key;
	std::string value;
	std::string line;
	std::string lineTrim;
	std::string delimiter = "\n";
	size_t pos = 0;

	if (raw == "")
		raw = this->_raw;

	if ((pos = raw.find_first_of(delimiter)) != std::string::npos)
    {
        line = raw.substr(0, pos + 1);
        raw.erase(0, pos + 1);
        this->_firstLine = line;
        line.clear();
    }

	while ((pos = raw.find_first_of(delimiter)) != std::string::npos)
	{
		line = raw.substr(0, pos + 1);
        lineTrim = trim(line, "\n\r");
        std::transform(lineTrim.begin(), lineTrim.end(), lineTrim.begin(), ::tolower);
		raw.erase(0, pos + 1);
        this->_lines.push_back(lineTrim);
	}
	this->parseFirstLine();
	this->parseKeyValue();
	this->parseBody();
}

void ParserRequest::parseFirstLine()
{
	std::string method;
	std::string path;
	std::string version;
	std::stringstream sp(this->_firstLine);

	sp >> std::ws >> method;
	sp >> std::ws >> path;
	sp >> std::ws >> version;

	this->request.setPath(path);
	this->request.setVersion(version);
	this->request.setHttpMethodName(method);
}

void ParserRequest::parseKeyValue()
{
	std::string line;
	std::string key;
	std::string value;

	for (size_t i = 0; i < this->_lines.size(); i++)
	{
		line = this->_lines[i];
		key = findKey(line);
		value = findValue(line);
		if (this->request.getKeywordDataBase().isHere<IsVariableNameEqual>(key))
		{
			this->request.getKeywordDataBase().updateData<IsVariableNameEqual, std::string>(key, value);
		}
	}
}

void ParserRequest::parseBody()
{
	this->parsedRequest = this->request.clone();
}

std::string ParserRequest::findKey(const std::string &line)
{
	std::string key;
	size_t i = line.find_first_of(':');

	key.append(line, 0, i);
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	return (trim(key, " \n\r\t\f\v"));
}
std::string ParserRequest::findValue(const std::string &line)
{
	size_t i;
	std::string value;

	i = line.find_first_of(':');
	i = line.find_first_not_of(' ', i + 1);
	if (i != std::string::npos)
		value.append(line, i, std::string::npos);
    return (trim(value, " \n\r\t\f\v"));
}
std::vector<std::string> ParserRequest::getLines()
{
    return (this->_lines);
}
std::string ParserRequest::getRaw()
{
    return (this->_raw);
}
void ParserRequest::setRaw(std::string raw)
{
    this->_raw = raw;
}
Request *ParserRequest::getRequest()
{
    return (this->parsedRequest);
}