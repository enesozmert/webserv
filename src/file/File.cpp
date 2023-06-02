#include "../inc/file/File.hpp"

File::File() {}

File::~File() {}

File::File(const File &file)
{
    *this = file;
}

File::File(const std::string& path) : _file(path.c_str())
{
    if (!_file) {
        this->_configException.run(101);
    }
    if (!_file.is_open()) {
        this->_configException.run(101);
    }
}

File	&File::operator=(const File &file)
{
    if (this == &file)
        return (*this);
    this->_path = file._path;
    this->_configException = file._configException;
    return (*this);
}

std::string File::readToString()
{
    std::string fileContents((std::istreambuf_iterator<char>(_file)), std::istreambuf_iterator<char>());
    if (fileContents.empty())
        this->_configException.run(102);
    return fileContents;
}

bool File::checkFileExtension(const std::string& filePath, const std::string& extension) {
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos) {
        this->_configException.run(105);
        return false;
    }
    std::string fileExtension = filePath.substr(dotPos + 1);
    if (fileExtension.empty())
        this->_configException.run(103);
    return (fileExtension == extension);
}

std::string File::getPath()
{
    return (this->_path);
}

void File::close()
{
    this->_file.close();
}