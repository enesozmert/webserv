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

        ~DataBase() {}

        std::vector<T> getAllData()
        {
            return (this->_data);
        }

        void insertData(T data)
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

        template <typename V, typename K>
        T getByNameData(V name)
        {
            T classe;
            typename std::vector<T>::iterator it = std::find_if(this->_data.begin(), this->_data.end(), K(name));

            if (it != this->_data.end())
            {
                return *it;
            }
            return classe;
        }

        template <typename V,typename F>
        bool isHere(V name)
        {
            return (std::find_if(_data.begin(), _data.end(), F(name)) != this->_data.end());
        }

        int size()
        {
            return (_data.size());
        }

};
