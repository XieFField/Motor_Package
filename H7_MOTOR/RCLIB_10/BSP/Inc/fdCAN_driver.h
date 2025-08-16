#ifndef FDCAN_DRIVER
#define FDCAN_DRIVER

        #pragma once
#include <cstdint>
#include "tool.h"

#ifdef __cplusplus
extern "C" 
{
    #include "fdcan.h"
    #include "gpio.h"

    typedef unsigned int uint32_t;
}
#endif

#define BUS_MAX_COUNT 3 // ���CAN��������
#define MAX_CAN_DEVICE 12 // ÿ��CAN��������豸����,12������������ȡ��

#ifdef __cplusplus



enum CAN_FrameType{
    CAN_FRAME_STD = 0, //��׼֡
    CAN_FRAME_EXT = 1  //��չ֡
};

//god knows
class CAN_Driver{
public:
    CAN_Driver(FDCAN_HandleTypeDef *hfdcan, CAN_FrameType CAN_Type, uint32_t CAN_ID)
    {
        AUTO_RegisterCANDriver();
    }

    uint32_t CAN_ID_;
    CAN_FrameType CAN_Type_;
    FDCAN_HandleTypeDef *hfdcan_;
    virtual void CAN_Update(uint8_t rx_buff[8]) = 0;

    static HAL_StatusTypeDef CAN_Send(FDCAN_HandleTypeDef *hfdcan, uint32_t can_id, 
                                                uint8_t isEXT, uint8_t tx_buff[8]);

    static HAL_StatusTypeDef CAN_FilterInit(void);

    static HAL_StatusTypeDef CAN_ConfigALL(FDCAN_HandleTypeDef *hfdcan);

    //���յ����ݷַ�����Ӧ�豸�� �ڻص������е���
    static void CAN_HandOut(FDCAN_HandleTypeDef *hcan, FDCAN_RxHeaderTypeDef *rxh, 
                                                               uint8_t rx_buff[8]);
private:
    /*
        ���ű�洢����ʵ������ÿ�����ߺ�֡����
    */
    static CAN_Driver *can_drivers_std [BUS_MAX_COUNT][MAX_CAN_DEVICE];
    static uint32_t can_driver_ids_std [BUS_MAX_COUNT][MAX_CAN_DEVICE];
    static uint8_t can_driver_count_std[BUS_MAX_COUNT];


    static CAN_Driver *can_drivers_ext [BUS_MAX_COUNT][MAX_CAN_DEVICE];
    static uint32_t can_driver_ids_ext [BUS_MAX_COUNT][MAX_CAN_DEVICE];
    static uint8_t can_driver_count_ext[BUS_MAX_COUNT];

    void AUTO_RegisterCANDriver(); //�����캯�����ú��Զ�ע��CANʵ��

    static int CAN_RegisterDriver(FDCAN_HandleTypeDef *driver);

    static void CAN1_ID_Repeated(void);
    static void CAN2_ID_Repeated(void);
    static void CAN3_ID_Repeated(void);
};


#endif


#endif
