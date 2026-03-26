#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include "camera/core/CoreResult.hpp"

namespace camera
{
    namespace core
    {

        // 布局尺寸限制常量
        constexpr int kMinLayoutDimension = 1;
        constexpr int kMaxLayoutDimension = 10;

        struct CameraSlot
        {
            std::string userId;
            int row = -1;
            int col = -1;
            std::string deviceInternalId;
        };

        class CameraLayout
        {
        public:
            CoreResult SetDimensions(int rows, int cols);
            CoreResult AddCamera(const CameraSlot& slot);

            CoreResult GetUserId(int row, int col, std::string* outUserId) const;
            CoreResult GetPosition(const std::string& userId, int* outRow, int* outCol) const;

            int rows() const
            {
                return m_rows;
            }
            int cols() const
            {
                return m_cols;
            }
            std::size_t cameraCount() const
            {
                return m_slots.size();
            }
            const std::vector<CameraSlot>& slots() const
            {
                return m_slots;
            }

        private:
            bool IsInBounds(int row, int col) const;

            int m_rows = 0;
            int m_cols = 0;
            std::vector<CameraSlot> m_slots;
            std::unordered_map<std::string, std::size_t> m_idToIndex;
            std::unordered_map<long long, std::size_t> m_positionToIndex;
        };

    } // namespace core
} // namespace camera
