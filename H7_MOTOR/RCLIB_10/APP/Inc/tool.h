#ifndef TOOL_H
#define TOOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
int binarySearch(const uint32_t arr[], uint8_t count, uint32_t key);

#ifdef __cplusplus
}
#endif

/**
 * @brief ¼ÆËã¾ø¶ÔÖµ
 */
template<typename Type> 
Type _tool_Abs(Type x) 
{
    return ((x > 0) ? x : -x);
}

#endif // TOOL_H