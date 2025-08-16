/**
 * @file fdCAN_driver.cpp
 * @author XieFField(WJ)
 * @brief fdCAN驱动文件
 * 
 *        重写此驱动旨在优化挑战赛所用代码框架所存在的问题
 *        采用cpp架构可以更有效解决，直接将所用设备实例化，
 *        可以简单粗暴地解决RM电机CAN帧被吞的问题,也不需要
 *        让全局变量在各个文件中漫天飞，相对优雅。
 */

#include "fdCAN_driver.h"


CAN_Driver *CAN_Driver::can_drivers_std[BUS_MAX_COUNT][MAX_CAN_DEVICE] = {{0}};
uint32_t    CAN_Driver::can_driver_ids_std[BUS_MAX_COUNT][MAX_CAN_DEVICE] = {{0}};
uint8_t     CAN_Driver::can_driver_count_std[BUS_MAX_COUNT] = {0};


CAN_Driver *CAN_Driver::can_drivers_ext[BUS_MAX_COUNT][MAX_CAN_DEVICE] = {{0}};
uint32_t    CAN_Driver::can_driver_ids_ext[BUS_MAX_COUNT][MAX_CAN_DEVICE] = {{0}};
uint8_t     CAN_Driver::can_driver_count_ext[BUS_MAX_COUNT] = {0};

void CAN_Driver::CAN1_ID_Repeated(void)
{
    // 处理CAN1的ID重复问题
    while(1){}
}

void CAN_Driver::CAN2_ID_Repeated(void)
{
    // 处理CAN2的ID重复问题
    while(1){}
}

void CAN_Driver::CAN3_ID_Repeated(void)
{
    // 处理CAN3的ID重复问题
    while(1){}
}

int CAN_Driver::CAN_RegisterDriver(CAN_Driver *driver)
{
    if (driver == nullptr) 
        return -1;

    else if(driver == &hfdcan1)
        return 0;

    else if(driver == &hfdcan2)
        return 1;

    else if(driver == &hfdcan3)
        return 2;

    // H723只有3路CAN
    else
        return -1;
}

void CAN_Driver::AUTO_RegisterCANDriver()
{
    int bus = CAN_RegisterDriver(this);
    if (bus < 0 || bus >= BUS_MAX_COUNT)   
        return;
    // 选择对应的设备列表和ID数组

    if(CAN_Type_ == CAN_FRAME_STD)
    {
        if(can_driver_count_std[bus] > MAX_CAN_DEVICE)
            return;
        
        
    }
    else
    {
        
    }
    
}

CAN_Driver::CAN_Driver(FDCAN_HandleTypeDef *hfdcan, CAN_FrameType CAN_Type, uint32_t CAN_ID)
{
    AUTO_RegisterCANDriver();
}
