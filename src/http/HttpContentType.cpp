#include "../inc/http/HttpContentType.hpp"

HttpContentType::HttpContentType()
{
    keywordFill();
}
HttpContentType::~HttpContentType()
{
    
}
HttpContentType::HttpContentType(const HttpContentType &httpContentType)
{
    *this = httpContentType;
}
bool            HttpContentType::checkType(std::string type)
{
    if (this->getKeywordDataBase().isHere<std::string, IsContentTypeTypeEqual>(type))
    {
        return (true);
    }
    return (false);
}
HttpVariable<std::string, std::string> HttpContentType::getByType(std::string type)
{
    if (!this->getKeywordDataBase().isHere<std::string, IsContentTypeTypeEqual>(type))
    {
        HttpVariable<std::string, std::string> statusCode;
        statusCode.setKey("none");
        statusCode.setValue("none");
        return (statusCode);
    }
    return (this->getKeywordDataBase().getByNameData<std::string, IsContentTypeTypeEqual>(type));
}
std::string HttpContentType::contentTypeGenerator(std::string type)
{
    std::string result;
    if (!this->getKeywordDataBase().isHere<std::string, IsContentTypeTypeEqual>(type))
    {
        HttpVariable<std::string, std::string> statusCode;
        statusCode.setKey("html");
        statusCode.setValue("text");
        result = statusCode.getValue() + "/" + statusCode.getKey();
        return (result);
    }
    HttpVariable<std::string, std::string> statusCode;
    statusCode = this->getKeywordDataBase().getByNameData<std::string, IsContentTypeTypeEqual>(type);
    result = statusCode.getValue() + "/" + statusCode.getKey();
    return (result);
}
DataBase<HttpVariable<std::string, std::string> > HttpContentType::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}
void    HttpContentType::setKeywordDatabase(DataBase<HttpVariable<std::string, std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}
void    HttpContentType::keywordFill()
{
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("html", "text"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("css", "text"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("javascript", "text"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("plain", "text"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("json", "application"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("xml", "application"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("pdf", "application"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("zip", "application"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("octet-stream", "application"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("x-www-form-urlencoded", "application"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("jpeg", "image"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("jpg", "image"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("png", "image"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("gif", "image"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("bmp", "image"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("mpeg", "audio"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("wav", "audio"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("mp4", "video"));
    _keywordDatabase.insertData(HttpVariable<std::string, std::string>("mpeg", "video"));
}