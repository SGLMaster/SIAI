#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace Util
{
    namespace String
    {
        template<typename STLContainer>
        void splitIntoContainer(const std::string& str, STLContainer&& container, char delimiter)
        {
            std::istringstream stream(str);

            std::string temporalString;
            while(std::getline(stream, temporalString, delimiter))
            {
                container.push_back(temporalString);
            }
        }

        template<typename STLContainer>
        auto split(const std::string& str, char delimiter)
        {
            STLContainer container{};

            std::istringstream stream(str);

            std::string temporalString;
            while(std::getline(stream, temporalString, delimiter))
            {
                container.push_back(temporalString);
            }

            return container;
        }
    }
}
