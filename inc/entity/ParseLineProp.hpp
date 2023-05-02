#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <string>

class ParseLineProp
{
private:
    size_t _index;
    size_t _scopeOpenIndex;
    size_t _scopeCloseIndex;
    std::string _scopeName;
    std::string _line;
    bool _isScopeOpen;
    bool _isScopeClose;

public:
    ParseLineProp(/* args */) {}
    ParseLineProp(const ParseLineProp &other) : _index(other._index),
                                                _scopeOpenIndex(other._scopeOpenIndex), _scopeCloseIndex(other._scopeCloseIndex),
                                                _scopeName(other._scopeName), _line(other._line)
    {
    }
    ~ParseLineProp() {}
    size_t getIndex() const { return _index; }
    size_t getScopeOpenIndex() const
    {
        return _scopeOpenIndex;
    }
    size_t getScopeCloseIndex() const
    {
        return _scopeCloseIndex;
    }
    std::string getScopeName() const
    {
        return _scopeName;
    }
    std::string getLine() const
    {
        return _line;
    }

    bool getIsScopeOpen()
    {
        return _isScopeOpen;
    }

    bool getIsScopeClose()
    {
        return _isScopeClose;
    }

    void setIndex(size_t _index)
    {
        this->_index = _index;
    }

    void setScopeOpenIndex(size_t _scopeOpenIndex)
    {
        this->_scopeOpenIndex = _scopeOpenIndex;
    }

    void setScopeCloseIndex(size_t _scopeCloseIndex)
    {
        this->_scopeCloseIndex = _scopeCloseIndex;
    }

    void setScopeName(std::string _scopeName)
    {
        this->_scopeName = _scopeName;
    }

    void setLine(std::string _line)
    {
        this->_line = _line;
    }

    void setIsScopeOpen(bool isScopeOpen)
    {
        this->_isScopeOpen = isScopeOpen;
        std::cout << "_isScopeOpen " << _isScopeOpen << std::endl;
    }

    void setIsScopeClose(bool isScopeClose)
    {
        this->_isScopeClose = isScopeClose;
    }
};

inline bool compareByScopeOpenIndex(const ParseLineProp &a, const ParseLineProp &b)
{
    return a.getScopeCloseIndex() < b.getScopeCloseIndex();
}