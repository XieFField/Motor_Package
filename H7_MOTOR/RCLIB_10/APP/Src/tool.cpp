/**
 * @file tool.cpp
 * @author XieFField
 * @brief 工具箱，提供一些常用的工具函数
 */
#include "tool.h"

/**
 * @brief 在有序数组中快速查找目标值的索引
 */
int binarySearch(const uint32_t arr[], uint8_t count, uint32_t key)
{
    int low = 0, high = count - 1;
    while (low <= high)
    {
        int mid = (low + high) >> 1;
        if (arr[mid] == key)
            return mid;
        if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}