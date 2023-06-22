#pragma once

#include "../server/Server.hpp"

/* inline int getMatchLocationPathIndex(ServerScope *matchedServerScope, std::string path) 
{
    std::vector<LocationScope *> selectedLocationScope;
    int index = -1;
    std::string trimmedPath;
    std::string trimmedSelectedPath;

    selectedLocationScope = matchedServerScope->getLocations(); 
    for (std::vector<LocationScope *>::iterator it = selectedLocationScope.begin(); it != selectedLocationScope.end(); it++)
    {
        index++;
        std::cout << PURPLE <<  "location index = " << index << RESET << std::endl;
        std::cout << PURPLE <<  "(*it)->getPath() = " << (*it)->getPath() << RESET << std::endl;
        std::cout << PURPLE <<  "gelen path = " << path << RESET << std::endl;
        trimmedSelectedPath = trim((*it)->getPath(), "\n\r\t ");
        trimmedPath = trim(path, "\n\r\t ");
        if (trimmedSelectedPath == trimmedPath || trimmedPath == "/favicon.ico")
                return(index);
        }
    return (-1);
}
 */

inline int getMatchLocationPathIndex(ServerScope *matchedServerScope, std::string path) 
{
    std::vector<LocationScope *> selectedLocationScope;
    int index = -1;
    int defaultLocation = -1;
    std::string trimmedPath;
    std::string trimmedSelectedPath;

    selectedLocationScope = matchedServerScope->getLocations(); 
    for (std::vector<LocationScope *>::iterator it = selectedLocationScope.begin(); it != selectedLocationScope.end(); it++)
    {
        index++;
        trimmedSelectedPath = trim((*it)->getPath(), "\n\r\t ");
        trimmedPath = trim(path, "\n\r\t ");
        if (trimmedSelectedPath == trimmedPath || trimmedSelectedPath == "/")
        {
            if (trimmedSelectedPath == "/")
                defaultLocation = index;
            else if (trimmedSelectedPath == trimmedPath)
                return(index);
        }
    }
    if(defaultLocation != -1)
        return(defaultLocation);
    return (-1);
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