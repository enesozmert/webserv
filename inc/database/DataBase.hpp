#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

template <typename T>
class DataBase
{
    private:

    public:
        std::vector<T> _data;    

        DataBase(){}
        DataBase(std::vector<T> data)
        {
            this->_data = data;
        }
        ~DataBase()
        {

        }

        void insertData(T data) // add new element
        {
            this->_data.push_back(data);
        }

        void updateData(T oldName, T newName)
        {
            typename std::vector<T>::iterator it = std::find(this->_data.begin(), this->_data.end(), oldName);

            if (it != this->_data.end())
                *it = newName;
        }

        void deleteData(T dataName)
        {
            typename std::vector<T>::iterator it = std::find(this->_data.begin(), this->_data.end(), dataName);
            if (it != this->_data.end()) {
                this->_data.erase(it);
            }
        }

        bool isHere(std::function<bool(T)> event)
        {
            return (std::find_if(_data.begin(), _data.end(), event) != this->_data.end());
        }

        int size()
        {
            return (_data.size());
        }

};


/* int main(){
    
    std::vector<std::string> vec;
    vec.push_back("aa");
    vec.push_back("bb");
    vec.push_back("cc");
    DataBase <std::string>db(vec);
    db.updateData("cc", "qq");
    std::cout<<db.isHere("qq")<<std::endl;
    std::cout<<db.isHere("merhaba")<<std::endl;
    
} */