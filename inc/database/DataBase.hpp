#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "../entity/Variable.hpp"
#include "../function/VariableFunc.hpp"

template <typename T>
class DataBase
{
    private:

    public:
        typedef bool (*EventFunction)(T);
        std::vector<T> _data;    

        DataBase(){}
        DataBase(std::vector<T> data)
        {
            this->_data = data;
        }
        ~DataBase()
        {

        }

        std::vector<T> getAllData()
        {
            return (this->_data);
        }

        void insertData(T data) // add new element
        {
            this->_data.push_back(data);
        }
        
        template <typename K, typename M>
        void updateData(std::string name, std::string newValue)
        {
            typename std::vector<Variable<M> >::iterator it = std::find_if(this->_data.begin(), this->_data.end(), K(name));

            if (it != this->_data.end())
            {
                *(it->getValue()) = newValue;
            }
        }

        void deleteData(T dataName)
        {
            typename std::vector<T>::iterator it = std::find(this->_data.begin(), this->_data.end(), dataName);
            if (it != this->_data.end()) {
                this->_data.erase(it);
            }
        }

        template <typename K>
        T getByNameData(std::string name)
        {
            T classe;
            typename std::vector<T>::iterator it = std::find_if(this->_data.begin(), this->_data.end(), K(name));

            if (it != this->_data.end())
            {
                return *it;
            }
            return classe;
        }

        template <typename K>
        bool isHere(std::string name)
        {
            return (std::find_if(_data.begin(), _data.end(), K(name)) != this->_data.end());
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