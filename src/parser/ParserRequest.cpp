#include "../inc/parser/ParserRequest.hpp"

ParserRequest::ParserRequest() {
	this->status = 0;
}

ParserRequest::~ParserRequest() {}

int	ParserRequest::getStatus() const { return status; }

int	ParserRequest::getMulti() const { return this->_multi; }

int ParserRequest::parse(std::string raw)
{
	this->_raw = raw;
	writeRequest();
	if (this->_raw.find("multi") != std::string::npos){
        this->_multi = 1;
	}
    else{
        this->_multi = 0;
	}
	this->status = 0;
    std::string key;
	std::string value;
	std::string line;
	std::string lineTrim;
	std::string delimiter = "\n";
	std::string tempRaw;
	size_t pos = 0;

	if (raw == "")
		raw = this->_raw;
	size_t endIndex = this->_raw.find("\r\n\r\n");
	tempRaw = raw.substr(0, endIndex);
	tempRaw += "\n";

	if ((pos = tempRaw.find_first_of(delimiter)) != std::string::npos)
    {
        line = tempRaw.substr(0, pos + 1);
        tempRaw.erase(0, pos + 1);
        this->_firstLine = line;
        line.clear();
    }

	while ((pos = tempRaw.find_first_of(delimiter)) != std::string::npos)
	{
		line = tempRaw.substr(0, pos + 1);
        lineTrim = trim(line, "\n\r");
        this->_lines.push_back(lineTrim);
		tempRaw.erase(0, pos + 1);
	}
	parseFirstLine();
	parseKeyValue();
	parseBody();
	parseRequestClone();
	this->status = 1;
	return status;
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
		if (this->request.getKeywordDataBase().isHere<std::string, IsVariableNameEqual>(key))
		{
			this->request.getKeywordDataBase().updateData<IsVariableNameEqual, std::string>(key, value);
		}
	}
}

void ParserRequest::parseBody()
{
	this->request.setBody(this->_raw.substr(this->_raw.find("\r\n\r\n") + 4));
}

void ParserRequest::parseRequestClone()
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

void ParserRequest::writeRequest()
{
	std::string request(_raw);
     if (_raw.size() < 1000)
        std::cout << PURPLE << "\nRequest :" << std::endl << "[" << request << "]" << RESET << std::endl;
    else
        std::cout << PURPLE << "\nRequest :" << std::endl << "[" << request.substr(0, 1500) << "..." << "]" << RESET << std::endl;

}