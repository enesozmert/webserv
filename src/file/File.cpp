#include "../inc/file/File.hpp"

File::File()
{

}

File::~File()
{
}

File::File(const std::string& fileName) : _file(fileName.c_str())
{
    if (!_file.is_open()) {
            throw std::runtime_error("Could not open file");
        }
}

std::string File::readToString()
{
    std::string fileContents((std::istreambuf_iterator<char>(_file)), std::istreambuf_iterator<char>());
    return fileContents;
}

void File::close()
{
    this->_file.close();
}