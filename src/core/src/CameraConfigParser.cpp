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
            /**
             * @brief 将字符串解析为整数
             * @param text 输入字符串
             * @param outValue 输出整数指针
             * @return CoreResult 解析结果
             */
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

            /**
             * @brief 在文本中搜索第一个匹配正则表达式的内容
             * @param text 输入文本
             * @param re 正则表达式
             * @param outMatch 输出匹配结果
             * @return true 如果找到匹配
             */
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

            // 打开配置文件
            std::ifstream input(filePath);
            if (!input.is_open())
            {
                return CoreResult::Failure(CoreErrorCode::kNotFound, "config file not found: " + filePath);
            }

            // 读取整个文件内容到字符串
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

            // 解析厂商类型，格式：<vendorType>xxx</vendorType>
            static const std::regex kVendorRe(R"re(<vendorType>\s*([^<\s]+)\s*</vendorType>)re");
            if (!MatchFirst(xmlText, kVendorRe, &match))
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "vendorType is missing");
            }
            outConfig->vendorType = match[1].str();

            // 解析布局尺寸，格式：<layout rows="x" cols="y" />
            static const std::regex kLayoutRe(R"re(<layout\s+rows="([0-9]+)"\s+cols="([0-9]+)"\s*/>)re");
            if (!MatchFirst(xmlText, kLayoutRe, &match))
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "layout rows/cols is missing");
            }

            // 解析行列数值
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

            // 设置布局尺寸
            CoreResult dimResult = outLayout->SetDimensions(outConfig->rows, outConfig->cols);
            if (!dimResult.ok())
            {
                return dimResult;
            }

            // 解析相机列表，格式：<camera userId="xxx" deviceId="yyy" row="r" col="c" />
            outConfig->cameras.clear();
            static const std::regex kCameraRe(
                R"re(<camera\s+userId="([^"]+)"\s+deviceId="([^"]*)"\s+row="([0-9]+)"\s+col="([0-9]+)"\s*/>)re");

            auto begin = std::sregex_iterator(xmlText.begin(), xmlText.end(), kCameraRe);
            auto end = std::sregex_iterator();

            if (begin == end)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "camera entries are missing");
            }

            // 遍历所有相机配置
            for (auto it = begin; it != end; ++it)
            {
                CameraDescriptor descriptor;
                descriptor.userId = (*it)[1].str();
                descriptor.deviceInternalId = (*it)[2].str();

                // 解析行号
                CoreResult rowParse = ParseInt((*it)[3].str(), &descriptor.row);
                if (!rowParse.ok())
                {
                    return rowParse;
                }
                // 解析列号
                CoreResult colParse = ParseInt((*it)[4].str(), &descriptor.col);
                if (!colParse.ok())
                {
                    return colParse;
                }

                outConfig->cameras.push_back(descriptor);

                // 将相机添加到布局
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
