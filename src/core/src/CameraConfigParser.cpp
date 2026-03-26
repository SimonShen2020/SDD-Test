// Module: Core
// Responsibility boundary: Parse camera xml config into topology and descriptors.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CameraConfigParser.hpp"

#include <tinyxml2.h>

#include <cstdio>
#include <fstream>
#include <sstream>

namespace camera::core
{

    namespace
    {
        std::string BuildErrorMessage(const tinyxml2::XMLDocument& doc, const char* context)
        {
            char buffer[256];
            std::snprintf(buffer, sizeof(buffer), "%s (line %d): %s",
                context,
                doc.ErrorLineNum(),
                doc.ErrorStr());
            return std::string(buffer);
        }

        CoreResult ParseIntAttribute(tinyxml2::XMLElement* element, const char* name, int* outValue)
        {
            if (element == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "element is null");
            }

            tinyxml2::XMLError error = element->QueryIntAttribute(name, outValue);
            if (error != tinyxml2::XML_SUCCESS)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument,
                    std::string("failed to parse attribute '") + name + "'");
            }
            return CoreResult::Success();
        }
    } // namespace

    CoreResult CameraConfigParser::ParseFromFile(const std::string& filePath, CameraConfig* outConfig,
        CameraLayout* outLayout) const
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

    CoreResult CameraConfigParser::ParseFromString(const std::string& xmlText, CameraConfig* outConfig,
        CameraLayout* outLayout) const
    {
        if (outConfig == nullptr || outLayout == nullptr)
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "output pointer is null");
        }

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError parseError = doc.Parse(xmlText.c_str());
        if (parseError != tinyxml2::XML_SUCCESS)
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument,
                BuildErrorMessage(doc, "XML parse error"));
        }

        tinyxml2::XMLElement* root = doc.RootElement();
        if (root == nullptr)
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "missing root element");
        }

        if (std::strcmp(root->Name(), "cameraConfig") != 0)
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument,
                std::string("expected root element 'cameraConfig', found '") + root->Name() + "'");
        }

        tinyxml2::XMLElement* vendorElem = root->FirstChildElement("vendorType");
        if (vendorElem == nullptr || vendorElem->GetText() == nullptr)
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "vendorType element is missing or empty");
        }
        outConfig->vendorType = vendorElem->GetText();

        tinyxml2::XMLElement* layoutElem = root->FirstChildElement("layout");
        if (layoutElem == nullptr)
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "layout element is missing");
        }

        CoreResult rowResult = ParseIntAttribute(layoutElem, "rows", &outConfig->rows);
        if (!rowResult.ok())
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "layout rows attribute: " + rowResult.message);
        }

        CoreResult colResult = ParseIntAttribute(layoutElem, "cols", &outConfig->cols);
        if (!colResult.ok())
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "layout cols attribute: " + colResult.message);
        }

        CoreResult dimResult = outLayout->SetDimensions(outConfig->rows, outConfig->cols);
        if (!dimResult.ok())
        {
            return dimResult;
        }

        outConfig->cameras.clear();

        tinyxml2::XMLElement* camerasElem = root->FirstChildElement("cameras");
        if (camerasElem == nullptr)
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "cameras element is missing");
        }

        tinyxml2::XMLElement* cameraElem = camerasElem->FirstChildElement("camera");
        if (cameraElem == nullptr)
        {
            return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "no camera entries found");
        }

        for (; cameraElem != nullptr; cameraElem = cameraElem->NextSiblingElement("camera"))
        {
            CameraDescriptor descriptor;

            const char* userId = cameraElem->Attribute("userId");
            if (userId == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "camera element missing userId attribute");
            }
            descriptor.userId = userId;

            const char* deviceId = cameraElem->Attribute("deviceId");
            if (deviceId != nullptr)
            {
                descriptor.deviceInternalId = deviceId;
            }

            CoreResult rowParse = ParseIntAttribute(cameraElem, "row", &descriptor.row);
            if (!rowParse.ok())
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument,
                    "camera '" + descriptor.userId + "' row attribute: " + rowParse.message);
            }

            CoreResult colParse = ParseIntAttribute(cameraElem, "col", &descriptor.col);
            if (!colParse.ok())
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument,
                    "camera '" + descriptor.userId + "' col attribute: " + colParse.message);
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
                return CoreResult::Failure(addResult.code,
                    "failed to add camera '" + descriptor.userId + "': " + addResult.message);
            }
        }

        return CoreResult::Success();
    }

} // namespace camera::core
