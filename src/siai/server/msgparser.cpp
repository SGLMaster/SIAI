#include "server/msgparser.hpp"

#include "util/string.hpp"

constexpr char CMD_VAL_SEPARATOR = ':';

std::string MsgParser::getCommand(const std::string& msg)
{
    return Util::String::getOptionName(msg, CMD_VAL_SEPARATOR);
}

std::string MsgParser::getValue(const std::string& msg)
{
    return Util::String::getOptValueAsStr(msg, CMD_VAL_SEPARATOR);
}