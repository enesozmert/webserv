#include "../inc/entity/Http.hpp"

Http::Http(/* args */)
{
    this->keywordFill();

}


Http::~Http()
{
}

std::string Http::getClientMaxBodySize()
{
    return (this->_clientMaxBodySize);
}
std::vector<Server> Http::getServer()
{
    return (this->_servers);
}
ErrorPage Http::getErrorPage()
{
    return (this->_errorPage);
}
DataBase<std::string> Http::getKeywordDataBase()
{
    return (this->_keywordDatabase);
}

void Http::setClientMaxBodySize(std::string clientMaxBodySize)
{
    this->_clientMaxBodySize = clientMaxBodySize;
}
void Http::setServer(Server server)
{
    this->_servers.push_back(server);
}
void Http::setErrorPage(ErrorPage errorPage)
{
    this->_errorPage = errorPage;
}
void Http::setKeywordDatabase(DataBase<std::string> keywordDatabase)
{
    this->_keywordDatabase = keywordDatabase;
}

void Http::keywordFill()
{

    std::vector<std::string>  datas;

    datas.push_back("include");
    datas.push_back("index");
    datas.push_back("default_type");
    datas.push_back("index");
    datas.push_back("log_format");
    datas.push_back("access_log");
    datas.push_back("sendfile");
    datas.push_back("tcp_nopush");
    datas.push_back("server_names_hash_bucket_size");

    this->setKeywordDatabase(datas);
}

std::string Http::getName() const
{
    return (this->name);
}

Http* Http::clone() const { return new Http(*this); }

