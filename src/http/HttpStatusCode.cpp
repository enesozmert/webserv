#include "../inc/http/HttpStatusCode.hpp"

HttpStatusCode::HttpStatusCode()
{
    keywordFill();
}

HttpStatusCode::~HttpStatusCode() {}

HttpStatusCode::HttpStatusCode(const HttpStatusCode &httpStatusCode)
{
    *this = httpStatusCode;
}
bool HttpStatusCode::checkStatusCode(int statusCode)
{
    if (this->getKeywordDataBase().isHere<int, IsStatusCodeVariableStatusCodeEqual>(statusCode))
    {
        return (true);
    }
    return (false);
}
HttpVariable<int, std::string> HttpStatusCode::getByStatusCode(int statusCode)
{
    if (!this->getKeywordDataBase().isHere<int, IsStatusCodeVariableStatusCodeEqual>(statusCode))
    {
        HttpVariable<int, std::string> statusCode;
        statusCode.setKey(-1);
        statusCode.setValue("Unknown Code");
        return (statusCode);
    }
    return (this->getKeywordDataBase().getByNameData<int, IsStatusCodeVariableStatusCodeEqual>(statusCode));
}
void HttpStatusCode::setKeywordDatabase(DataBase<HttpVariable<int, std::string> > keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}
DataBase<HttpVariable<int, std::string> > HttpStatusCode::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}
void HttpStatusCode::keywordFill()
{
    _keywordDatabase.insertData(HttpVariable<int, std::string>(100, "Continue"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(101, "Switching Protocols"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(200, "OK"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(201, "Created"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(202, "Accepted"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(203, "Non-Authoritative Information"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(204, "No Content"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(205, "Reset Content"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(206, "Partial Content"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(300, "Multiple Choices"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(301, "Moved Permanently"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(302, "Found"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(303, "See Other"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(304, "Not Modified"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(305, "Use Proxy"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(307, "Temporary Redirect"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(308, "Permanent Redirect"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(400, "Bad Request"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(401, "Unauthorized"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(402, "Payment Required"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(403, "Forbidden"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(404, "Not Found"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(405, "Method Not Allowed"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(406, "Not Acceptable"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(407, "Proxy Authentication Required"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(408, "Request Timeout"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(409, "Conflict"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(410, "Gone"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(411, "Length Required"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(412, "Precondition Failed"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(413, "Payload Too Large"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(414, "URI Too Long"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(415, "Unsupported Media Type"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(416, "Range Not Satisfiable"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(500, "Internal Server Error"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(503, "Service Unavailable"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(504, "Gateway Timeout"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(505,"HTTP Version Not Supported"));
    _keywordDatabase.insertData(HttpVariable<int, std::string>(511, "Network Authentication Required"));
}