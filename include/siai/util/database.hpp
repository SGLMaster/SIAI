#pragma once

#include <string>

class DbConnectionOptions;

namespace Util
{
    namespace Db
    {
        void saveDbOptionsToFile(const DbConnectionOptions& options, const std::string& filename);
        DbConnectionOptions loadDbOptionsFromFile(const std::string& filename);
    }
}