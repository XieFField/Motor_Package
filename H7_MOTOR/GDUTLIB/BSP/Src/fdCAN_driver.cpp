/**
 * @file fdCAN_driver.cpp
 * @author XieFField(WJ)
 * @brief fdCAN�����ļ�
 * 
 *        ��д������ּ���Ż���ս�����ô����������ڵ�����
 *        ����cpp�ܹ����Ը���Ч�����ֱ�ӽ������豸ʵ������
 *        ���Լ򵥴ֱ��ؽ��RM���CAN֡���̵�����,Ҳ����Ҫ
 *        ��ȫ�ֱ����ڸ����ļ�������ɣ�������š�
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
    // ����CAN1��ID�ظ�����
    while(1){}
}

void CAN_Driver::CAN2_ID_Repeated(void)
{
    // ����CAN2��ID�ظ�����
    while(1){}
}

void CAN_Driver::CAN3_ID_Repeated(void)
{
    // ����CAN3��ID�ظ�����
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

    // H723ֻ��3·CAN
    else
        return -1;
}

void CAN_Driver::AUTO_RegisterCANDriver()
{
    int bus = CAN_RegisterDriver(this);
    if (bus < 0 || bus >= BUS_MAX_COUNT)   
        return;
    // ѡ���Ӧ���豸�б��ID����

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
