#include "util/string.hpp"

bool Util::String::startsWith(const std::string& mainStr, const std::string& toMatch)
{
    if(mainStr.find(toMatch) == 0)
        return true;
            
    return false;
}

std::string Util::String::getOptionName(const std::string& line, char separator)
{
    size_t separatorPosition = line.find(separator);

    return trim(line.substr(0, separatorPosition));
}

int Util::String::getOptValueAsInt(const std::string& line, char separator)
{
    std::string tmpStr = getOptValueAsStr(line, separator);
    long tmpValue = strtol(tmpStr.c_str(), NULL, 10);

    return static_cast<int>(tmpValue);
}

std::string Util::String::getOptValueAsStr(const std::string& line, char separator)
{
    size_t separatorPosition = line.find(separator);

    return (separatorPosition == std::string::npos) ? "" : trim(line.substr(separatorPosition + 1));
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