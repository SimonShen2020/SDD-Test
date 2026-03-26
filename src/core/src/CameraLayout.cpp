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
            long long PositionKey(int row, int col)
            {
                return (static_cast<long long>(row) << 32) | static_cast<unsigned int>(col);
            }

            bool IsValidUserId(const std::string& userId)
            {
                static const std::regex kUserIdPattern("^[A-Z]{2,6}[A-Z][0-9]+$");
                return std::regex_match(userId, kUserIdPattern);
            }
        } // namespace

        CoreResult CameraLayout::SetDimensions(int rows, int cols)
        {
            if (rows < kMinLayoutDimension || cols < kMinLayoutDimension || rows > kMaxLayoutDimension || cols > kMaxLayoutDimension)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument,
                    "layout dimensions must be within [" + std::to_string(kMinLayoutDimension) + ", " + std::to_string(kMaxLayoutDimension) + "]");
            }

            m_rows = rows;
            m_cols = cols;
            m_slots.clear();
            m_idToIndex.clear();
            m_positionToIndex.clear();
            return CoreResult::Success();
        }

        CoreResult CameraLayout::AddCamera(const CameraSlot& slot)
        {
            if (m_rows == 0 || m_cols == 0)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "layout dimensions must be set before AddCamera");
            }

            if (!IsInBounds(slot.row, slot.col))
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "camera position is out of bounds");
            }

            if (!IsValidUserId(slot.userId))
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "userId does not match naming rule");
            }

            if (m_idToIndex.find(slot.userId) != m_idToIndex.end())
            {
                return CoreResult::Failure(CoreErrorCode::kAlreadyExists, "duplicated userId");
            }

            const long long key = PositionKey(slot.row, slot.col);
            if (m_positionToIndex.find(key) != m_positionToIndex.end())
            {
                return CoreResult::Failure(CoreErrorCode::kAlreadyExists, "duplicated position");
            }

            const std::size_t index = m_slots.size();
            m_slots.push_back(slot);
            m_idToIndex.emplace(slot.userId, index);
            m_positionToIndex.emplace(key, index);
            return CoreResult::Success();
        }

        CoreResult CameraLayout::GetUserId(int row, int col, std::string* outUserId) const
        {
            if (outUserId == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "outUserId is null");
            }

            const auto it = m_positionToIndex.find(PositionKey(row, col));
            if (it == m_positionToIndex.end())
            {
                return CoreResult::Failure(CoreErrorCode::kNotFound, "position not found");
            }

            *outUserId = m_slots[it->second].userId;
            return CoreResult::Success();
        }

        CoreResult CameraLayout::GetPosition(const std::string& userId, int* outRow, int* outCol) const
        {
            if (outRow == nullptr || outCol == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "output pointer is null");
            }

            const auto it = m_idToIndex.find(userId);
            if (it == m_idToIndex.end())
            {
                return CoreResult::Failure(CoreErrorCode::kNotFound, "userId not found");
            }

            const CameraSlot& slot = m_slots[it->second];
            *outRow = slot.row;
            *outCol = slot.col;
            return CoreResult::Success();
        }

        bool CameraLayout::IsInBounds(int row, int col) const
        {
            return row >= 0 && col >= 0 && row < m_rows && col < m_cols;
        }

    } // namespace core
} // namespace camera
