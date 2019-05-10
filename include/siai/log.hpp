#pragma once

#include <string>

namespace Log
{
    void simple(const std::string& msg, bool logToCmd=false);

    void timestamp(const std::string& msg, bool logToCmd=false);

    void warning(const std::string& msg, bool logToCmd=false);

    void error(const std::string& msg, bool logToCmd=false);
}
