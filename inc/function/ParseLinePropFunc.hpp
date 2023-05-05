#pragma once

#include "../entity/ParseLineProp.hpp"

inline bool compareByScopeOpenIndex(const ParseLineProp &a, const ParseLineProp &b)
{
    return a.getScopeCloseIndex() < b.getScopeCloseIndex();
}