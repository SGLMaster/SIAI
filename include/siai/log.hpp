#pragma once

#include <string>

namespace Log
{
    void simpleMessage(const std::string& msg, bool logToCmd=false);

    void warning(const std::string& msg, bool logToCmd=false);

    void fatalError(const std::string& msg, bool logToCmd=false);
}
