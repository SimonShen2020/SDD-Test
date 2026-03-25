// Module: Core
// Responsibility boundary: Manage camera matrix topology and userId mapping.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CameraLayout.hpp"

#include <regex>

namespace camera
{
    namespace core
    {

        namespace
        {
            /**
             * @brief 将二维行列坐标编码为 64 位整数键值
             *
             * 编码方式：高 32 位存储 row，低 32 位存储 col
             * 用于在 unordered_map 中快速定位相机位置
             *
             * @param row 行索引
             * @param col 列索引
             * @return 64 位编码键值
             */
            long long PositionKey(int row, int col)
            {
                return (static_cast<long long>(row) << 32) | static_cast<unsigned int>(col);
            }

            /**
             * @brief 验证 userId 是否符合命名规范
             *
             * 命名规则：2-6 个大写字母 + 1 个大写字母 + 1 个或多个数字
             * 有效示例：DHAA0, CAM001, ABCD123
             * 无效示例：dh001, A1, ABC
             *
             * @param userId 用户标识符
             * @return true 如果符合命名规范
             */
            bool IsValidUserId(const std::string& userId)
            {
                static const std::regex kUserIdPattern("^[A-Z]{2,6}[A-Z][0-9]+$");
                return std::regex_match(userId, kUserIdPattern);
            }
        } // namespace

        CoreResult CameraLayout::SetDimensions(int rows, int cols)
        {
            // 限制布局尺寸在合理范围内，防止内存过度分配
            if (rows < 1 || cols < 1 || rows > 10 || cols > 10)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "layout dimensions must be within [1, 10]");
            }

            rows_ = rows;
            cols_ = cols;
            // 清空已有数据，重新初始化布局
            slots_.clear();
            idToIndex_.clear();
            positionToIndex_.clear();
            return CoreResult::Success();
        }

        CoreResult CameraLayout::AddCamera(const CameraSlot& slot)
        {
            // 必须先设置布局尺寸才能添加相机
            if (rows_ == 0 || cols_ == 0)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "layout dimensions must be set before AddCamera");
            }

            // 检查相机位置是否在有效范围内
            if (!IsInBounds(slot.row, slot.col))
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "camera position is out of bounds");
            }

            // 验证 userId 命名规范
            if (!IsValidUserId(slot.userId))
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "userId does not match naming rule");
            }

            // 检查 userId 是否已存在（唯一性约束）
            if (idToIndex_.find(slot.userId) != idToIndex_.end())
            {
                return CoreResult::Failure(CoreErrorCode::kAlreadyExists, "duplicated userId");
            }

            // 检查位置是否已被占用（唯一性约束）
            const long long key = PositionKey(slot.row, slot.col);
            if (positionToIndex_.find(key) != positionToIndex_.end())
            {
                return CoreResult::Failure(CoreErrorCode::kAlreadyExists, "duplicated position");
            }

            // 添加相机到布局，维护三个索引结构
            const std::size_t index = slots_.size();
            slots_.push_back(slot);                    // 存储相机信息
            idToIndex_.emplace(slot.userId, index);    // userId -> 索引映射
            positionToIndex_.emplace(key, index);      // 位置 -> 索引映射
            return CoreResult::Success();
        }

        CoreResult CameraLayout::GetUserId(int row, int col, std::string* outUserId) const
        {
            if (outUserId == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "outUserId is null");
            }

            // 通过位置键值查找对应的 userId
            const auto it = positionToIndex_.find(PositionKey(row, col));
            if (it == positionToIndex_.end())
            {
                return CoreResult::Failure(CoreErrorCode::kNotFound, "position not found");
            }

            *outUserId = slots_[it->second].userId;
            return CoreResult::Success();
        }

        CoreResult CameraLayout::GetPosition(const std::string& userId, int* outRow, int* outCol) const
        {
            if (outRow == nullptr || outCol == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "output pointer is null");
            }

            // 通过 userId 查找对应的位置
            const auto it = idToIndex_.find(userId);
            if (it == idToIndex_.end())
            {
                return CoreResult::Failure(CoreErrorCode::kNotFound, "userId not found");
            }

            const CameraSlot& slot = slots_[it->second];
            *outRow = slot.row;
            *outCol = slot.col;
            return CoreResult::Success();
        }

        bool CameraLayout::IsInBounds(int row, int col) const
        {
            return row >= 0 && col >= 0 && row < rows_ && col < cols_;
        }

    } // namespace core
} // namespace camera
