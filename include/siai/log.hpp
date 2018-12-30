#pragma once

#include <string>

namespace Log
{
    void simpleMessage(const std::string& msg);

    void warning(const std::string& msg);

    void fatalError(const std::string& msg);
}
