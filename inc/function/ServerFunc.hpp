#pragma once

#include "../server/Server.hpp"

inline size_t getMatchLocationPathIndex(ServerScope *matchedServerScope, std::string path) 
{
    int defaultLocationScopeSize = 0;
    size_t result = 0;
    std::vector<LocationScope *> selectedLocationScope;

    selectedLocationScope = matchedServerScope->getLocations(); 
    for (std::vector<LocationScope *>::iterator it = selectedLocationScope.begin(); it != selectedLocationScope.end(); it++)
    {
        if ((*it)->getPath().compare(path) == 0)
        {
            defaultLocationScopeSize++;
            result++;
        }
    }
    if (defaultLocationScopeSize != 1)
    {
        //error
        return (0);
    }
    std::cout << "result getMatchLocationPathIndex : " << result << std::endl;
    return (result);
}

inline size_t getDefaultLocationPathIndex(ServerScope *matchedServerScope) {
    int defaultLocationScopeSize = 0;
    size_t result = 0;
    std::vector<LocationScope *> selectedLocationScope;

    selectedLocationScope = matchedServerScope->getLocations(); 
    for (std::vector<LocationScope *>::iterator it = selectedLocationScope.begin(); it != selectedLocationScope.end(); it++)
    {
        if ((*it)->getPath() == "/")
        {
            defaultLocationScopeSize++;
            result++;
        }
    }

    if (defaultLocationScopeSize != 1)
    {
        //error
        return (0);
    }
    std::cout << "result getDefaultLocationPathIndex : " << result << std::endl;
    return (result);
}

inline size_t getLongestLocationPathIndex(ServerScope *matchedServerScope) 
{
    size_t maxPathLength = 0;
    size_t result = 0;
    std::vector<LocationScope *> selectedLocationScope;

    selectedLocationScope = matchedServerScope->getLocations();
    for (std::vector<LocationScope *>::iterator it = selectedLocationScope.begin(); it != selectedLocationScope.end(); it++)
    {
        if ((*it)->getPath().length() > maxPathLength)
        {
            maxPathLength = (*it)->getPath().length();
            result++;
        }
    }
    std::cout << "result getLongestLocationPathIndex : " << result << std::endl;
    result--;
    return (result);
}