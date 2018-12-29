#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace Util
{
    namespace String
    {
        std::string generateCommand()
        {
            return "";
        }

        std::string generateCommand(int argument, auto&&... args)
        {
            return std::to_string(argument) + " " + generateCommand(args...);
        }

        std::string generateCommand(const std::string& argument, auto&&... args)
        {
            return std::string(argument) + " " + generateCommand(args...);
        }

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
