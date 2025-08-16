/**
 * @file fdCAN_driver.cpp
 * @author XieFField
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

int CAN_Driver::CAN_RegisterDriver(FDCAN_HandleTypeDef *driver)
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
    int bus = CAN_RegisterDriver(hfdcan_);
    if (bus < 0 || bus >= BUS_MAX_COUNT)   
        return;
    // 选择对应的设备列表和ID数组

    if(CAN_Type_ == CAN_FRAME_STD)
    {
        if(can_driver_count_std[bus] > MAX_CAN_DEVICE)
            return;
        
        int pos = can_driver_count_std[bus]++;
        int dup_idx = binarySearch(can_driver_ids_std[bus], can_driver_count_std[bus], 
                            CAN_ID_);
        if (dup_idx >= 0)
        {
            // ID重复，处理逻辑
            switch (bus)
            {
                case 0:
                    CAN1_ID_Repeated();
                    break;

                case 1:
                    CAN2_ID_Repeated();
                    break;

                case 2:
                    CAN3_ID_Repeated();
                    break;

                default:
                    break;
            }
        }

        //插入排序，保证id按序排列
        while(pos > 0 && can_driver_ids_std[bus][pos - 1] > CAN_ID_)
        {
            can_driver_ids_std[bus][pos] = can_driver_ids_std[bus][pos - 1];
            can_drivers_std[bus][pos] = can_drivers_std[bus][pos - 1];
            pos--;
        }
    }
    else
    {
        if(can_driver_count_ext[bus] > MAX_CAN_DEVICE)
            return;
        
        int pos = can_driver_count_ext[bus]++;

        int dup_idx = binarySearch(can_driver_ids_ext[bus], can_driver_count_ext[bus], 
                            CAN_ID_);
        if (dup_idx >= 0)
        {
            // ID重复，处理逻辑
            switch (bus)
            {
                case 0:
                    CAN1_ID_Repeated();
                    break;

                case 1:
                    CAN2_ID_Repeated();
                    break;

                case 2:
                    CAN3_ID_Repeated();
                    break;

                default:
                    break;
            }

            //插入排序，保证id按序排列
            while(pos > 0 && can_driver_ids_ext[bus][pos - 1] > CAN_ID_)
            {
                can_driver_ids_ext[bus][pos] = can_driver_ids_ext[bus][pos - 1];
                can_drivers_ext[bus][pos] = can_drivers_ext[bus][pos - 1];
                pos--;
            }
        }
    }
}

void CAN_Driver::CAN_HandOut(FDCAN_HandleTypeDef *hfdcan, FDCAN_RxHeaderTypeDef *rxhead, uint8_t *data)
{
    // 处理接收到的CAN消息
    int bus = CAN_RegisterDriver(hfdcan);

    uint32_t id = rxhead->Identifier;
    if(rxhead->IdType == FDCAN_STANDARD_ID)
    {
        // 处理标准帧
        int idx = binarySearch(can_driver_ids_std[bus], can_driver_count_std[bus], id);
        if (idx >= 0)
            can_drivers_std[bus][idx]->CAN_Update(data);
    }
    else
    {
        // 处理扩展帧
        int idx = binarySearch(can_driver_ids_ext[bus], can_driver_count_ext[bus], id);
        if (idx >= 0)
            can_drivers_ext[bus][idx]->CAN_Update(data);
    }
}

HAL_StatusTypeDef CAN_Driver::CAN_Send(FDCAN_HandleTypeDef *hfdcan, uint32_t can_id, 
                                                uint8_t isEXT, uint8_t tx_buff[8])
{
    FDCAN_TxHeaderTypeDef txHeader = {0};

    if(isEXT)
    {
        txHeader.IdType = FDCAN_EXTENDED_ID; // 设置为扩展帧
        txHeader.Identifier = can_id;        // 设置扩展 ID
    }
    else
    {
        txHeader.IdType = FDCAN_STANDARD_ID; // 设置为标准帧
        txHeader.Identifier = can_id;        // 设置标准 ID
    }

    txHeader.TxFrameType = FDCAN_DATA_FRAME; // 设置为数据帧
    txHeader.DataLength = FDCAN_DLC_BYTES_8; // 数据长度为 8 字节
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;   // 不使用全局时间戳
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;

    //发送
    return HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txHeader, tx_buff);
}

HAL_StatusTypeDef CAN_Driver::CAN_FilterInit(void)
{
    if (CAN_ConfigALL(&hfdcan2) != HAL_OK)
        return HAL_ERROR;

    if (CAN_ConfigALL(&hfdcan1) != HAL_OK)
        return HAL_ERROR;

    if (CAN_ConfigALL(&hfdcan3) != HAL_OK)
        return HAL_ERROR;

    HAL_FDCAN_Start(&hfdcan1); // 开启FDCAN
    HAL_FDCAN_Start(&hfdcan2);
    HAL_FDCAN_Start(&hfdcan3);
   
    //全接收模式
    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

    return HAL_OK;
}

HAL_StatusTypeDef CAN_Driver::CAN_ConfigALL(FDCAN_HandleTypeDef *hfdcan)
{
    FDCAN_FilterTypeDef sFilterConfig = {0};

    // 配置过滤器
    sFilterConfig.FilterType = FDCAN_FILTER_MASK; //MASK模式
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //放入FIFO0

    sFilterConfig.FilterID1 = 0x000; // 接收所有ID
    sFilterConfig.FilterID2 = 0x000; // 掩码为0，表示不关心ID位

    sFilterConfig.IdType = FDCAN_STANDARD_ID;   //std
    sFilterConfig.FilterIndex = 0;              

    HAL_StatusTypeDef result_std = HAL_FDCAN_ConfigFilter(hfdcan, &sFilterConfig);

    sFilterConfig.IdType = FDCAN_EXTENDED_ID;  //ext
    sFilterConfig.FilterIndex = 1;

    HAL_StatusTypeDef result_ext = HAL_FDCAN_ConfigFilter(hfdcan, &sFilterConfig);


    return (result_std == HAL_OK && result_ext == HAL_OK) ? HAL_OK : HAL_ERROR;
}

extern "C" void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    FDCAN_RxHeaderTypeDef rxhead;
    uint8_t data[8];
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxhead, data) != HAL_OK)
    {
        // 读取失败，跳过
        return;
    }

    CAN_Driver::CAN_HandOut(hfdcan, &rxhead, data);
}