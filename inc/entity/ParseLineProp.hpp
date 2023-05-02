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

    bool isScopeOpen() const
    {
        return _isScopeOpen;
    }

    bool isScopeClose() const
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

    void setIsScopeOpen(bool _isScopeOpen)
    {
        this->_isScopeOpen = _isScopeOpen;
    }

    void setIsScopeClose(bool _isScopeClose)
    {
        this->_isScopeClose = _isScopeClose;
    }
};

// bool compareByScopeOpenIndexSumScopeCloseIndex(const ParseLineProp &a, const ParseLineProp &b)
// {
//     return a.getScopeCloseIndex() + a.getScopeOpenIndex()  > b.getScopeCloseIndex() + b.getScopeOpenIndex();
// }