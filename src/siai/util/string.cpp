#include "util/string.hpp"

bool Util::String::startsWith(const std::string& mainStr, const std::string& toMatch)
{
    if(mainStr.find(toMatch) == 0)
        return true;
            
    return false;
}