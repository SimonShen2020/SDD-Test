// Module: Core
// Responsibility boundary: Parse camera xml config into topology and descriptors.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CameraConfigParser.hpp"

#include <fstream>
#include <regex>
#include <sstream>

namespace camera
{
    namespace core
    {

        namespace
        {
            CoreResult ParseInt(const std::string& text, int* outValue)
            {
                if (outValue == nullptr)
                {
                    return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "outValue is null");
                }

                try
                {
                    *outValue = std::stoi(text);
                    return CoreResult::Success();
                }
                catch (...)
                {
                    return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "invalid integer: " + text);
                }
            }

            bool MatchFirst(const std::string& text, const std::regex& re, std::smatch* outMatch)
            {
                return std::regex_search(text, *outMatch, re);
            }
        } // namespace

        CoreResult CameraConfigParser::ParseFromFile(const std::string& filePath, CameraConfig* outConfig, CameraLayout* outLayout) const
        {
            if (outConfig == nullptr || outLayout == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "output pointer is null");
            }

            std::ifstream input(filePath);
            if (!input.is_open())
            {
                return CoreResult::Failure(CoreErrorCode::kNotFound, "config file not found: " + filePath);
            }

            std::stringstream buffer;
            buffer << input.rdbuf();
            return ParseFromString(buffer.str(), outConfig, outLayout);
        }

        CoreResult CameraConfigParser::ParseFromString(const std::string& xmlText, CameraConfig* outConfig, CameraLayout* outLayout) const
        {
            if (outConfig == nullptr || outLayout == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "output pointer is null");
            }

            std::smatch match;

            static const std::regex kVendorRe(R"re(<vendorType>\s*([^<\s]+)\s*</vendorType>)re");
            if (!MatchFirst(xmlText, kVendorRe, &match))
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "vendorType is missing");
            }
            outConfig->vendorType = match[1].str();

            static const std::regex kLayoutRe(R"re(<layout\s+rows="([0-9]+)"\s+cols="([0-9]+)"\s*/>)re");
            if (!MatchFirst(xmlText, kLayoutRe, &match))
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "layout rows/cols is missing");
            }

            CoreResult rowResult = ParseInt(match[1].str(), &outConfig->rows);
            if (!rowResult.ok())
            {
                return rowResult;
            }

            CoreResult colResult = ParseInt(match[2].str(), &outConfig->cols);
            if (!colResult.ok())
            {
                return colResult;
            }

            CoreResult dimResult = outLayout->SetDimensions(outConfig->rows, outConfig->cols);
            if (!dimResult.ok())
            {
                return dimResult;
            }

            outConfig->cameras.clear();
            static const std::regex kCameraRe(
                R"re(<camera\s+userId="([^"]+)"\s+deviceId="([^"]*)"\s+row="([0-9]+)"\s+col="([0-9]+)"\s*/>)re");

            const auto begin = std::sregex_iterator(xmlText.begin(), xmlText.end(), kCameraRe);
            const auto end = std::sregex_iterator();
            if (begin == end)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "camera entries are missing");
            }

            for (auto it = begin; it != end; ++it)
            {
                CameraDescriptor descriptor;
                descriptor.userId = (*it)[1].str();
                descriptor.deviceInternalId = (*it)[2].str();

                CoreResult rowParse = ParseInt((*it)[3].str(), &descriptor.row);
                if (!rowParse.ok())
                {
                    return rowParse;
                }

                CoreResult colParse = ParseInt((*it)[4].str(), &descriptor.col);
                if (!colParse.ok())
                {
                    return colParse;
                }

                outConfig->cameras.push_back(descriptor);

                CameraSlot slot;
                slot.userId = descriptor.userId;
                slot.row = descriptor.row;
                slot.col = descriptor.col;
                slot.deviceInternalId = descriptor.deviceInternalId;

                CoreResult addResult = outLayout->AddCamera(slot);
                if (!addResult.ok())
                {
                    return addResult;
                }
            }

            return CoreResult::Success();
        }

    } // namespace core
} // namespace camera
