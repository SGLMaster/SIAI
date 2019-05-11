#include "util/string.hpp"

bool Util::String::startsWith(const std::string& mainStr, const std::string& toMatch)
{
    if(mainStr.find(toMatch) == 0)
        return true;
            
    return false;
}

int Util::String::getOptValueAsInt(const std::string& line)
{
    std::string tmpStr = getOptValueAsStr(line);
    long tmpValue = strtol(tmpStr.c_str(), NULL, 10);

    return static_cast<int>(tmpValue);
}

std::string Util::String::getOptValueAsStr(const std::string& line)
{
    return trim(line.substr(line.find("=") + 1));
}

std::string Util::String::trim(const std::string& str)
{
    return ltrim(rtrim(str));
}

const std::string WHITESPACE = " \n\r\t\f\v";

std::string Util::String::ltrim(const std::string& str)
{
    size_t start = str.find_first_not_of(WHITESPACE);

    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string Util::String::rtrim(const std::string& str)
{
    size_t end = str.find_last_not_of(WHITESPACE);

    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}