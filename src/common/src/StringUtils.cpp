// Module: Common
// Responsibility boundary: Provide string manipulation utility functions.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/common/StringUtils.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace camera
{
    namespace common
    {

        std::vector<std::string> Split(const std::string& input, char delimiter)
        {
            std::vector<std::string> output;
            std::stringstream stream(input);
            std::string item;
            while (std::getline(stream, item, delimiter))
            {
                output.push_back(item);
            }
            return output;
        }

        std::string Trim(const std::string& input)
        {
            auto beginIt = std::find_if_not(input.begin(), input.end(),
                [](unsigned char c)
                {
                    return std::isspace(c) != 0;
                });

            auto endIt = std::find_if_not(input.rbegin(), input.rend(),
                [](unsigned char c)
                {
                    return std::isspace(c) != 0;
                }).base();

            if (beginIt >= endIt)
            {
                return {};
            }

            return std::string(beginIt, endIt);
        }

    } // namespace common
} // namespace camera
