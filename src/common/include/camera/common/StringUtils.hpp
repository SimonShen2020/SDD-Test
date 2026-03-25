#pragma once

#include <string>
#include <vector>

namespace camera
{
    namespace common
    {

        std::vector<std::string> Split(const std::string& input, char delimiter);
        std::string Trim(const std::string& input);

    } // namespace common
} // namespace camera
