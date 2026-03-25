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
                return rows_;
            }
            int cols() const
            {
                return cols_;
            }
            std::size_t cameraCount() const
            {
                return slots_.size();
            }
            const std::vector<CameraSlot>& slots() const
            {
                return slots_;
            }

        private:
            bool IsInBounds(int row, int col) const;

            int rows_ = 0;
            int cols_ = 0;
            std::vector<CameraSlot> slots_;
            std::unordered_map<std::string, std::size_t> idToIndex_;
            std::unordered_map<long long, std::size_t> positionToIndex_;
        };

    } // namespace core
} // namespace camera
