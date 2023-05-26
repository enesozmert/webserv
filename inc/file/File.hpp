#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../error/ConfigException.hpp"

class ConfigException;
class File
{
    private:
        std::ifstream   _file;
        std::string     _path;
        ConfigException _configException;
    public:
        File();
        ~File();
        File(const File &file);
        File(const std::string& path);
        File	&operator=(const File &file);

        std::string readToString();
        void        close();
        bool        checkFileExtension(const std::string& filePath, const std::string& extension);
        std::string getPath();
};
