#pragma once

#include <string>

namespace MsgParser
{
    std::string getCommand(const std::string& msg);
    std::string getValue(const std::string& msg);
}