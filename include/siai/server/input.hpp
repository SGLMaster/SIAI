#pragma once

#include <string>

namespace CmdInput
{
    std::string getString(const std::string& name);
    int getUInt(const std::string& name);
}