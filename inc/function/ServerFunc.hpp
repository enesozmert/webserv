#pragma once

#include "../server/Server.hpp"

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