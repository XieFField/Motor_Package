/**
 * @file fdCAN_driver.cpp
 * @author XieFField
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

    // H723ֻ��3·CAN
    else
        return -1;
}

void CAN_Driver::AUTO_RegisterCANDriver()
{
    int bus = CAN_RegisterDriver(hfdcan_);
    if (bus < 0 || bus >= BUS_MAX_COUNT)   
        return;
    // ѡ���Ӧ���豸�б��ID����

    if(CAN_Type_ == CAN_FRAME_STD)
    {
        if(can_driver_count_std[bus] > MAX_CAN_DEVICE)
            return;
        
        int pos = can_driver_count_std[bus]++;
        int dup_idx = binarySearch(can_driver_ids_std[bus], can_driver_count_std[bus], 
                            CAN_ID_);
        if (dup_idx >= 0)
        {
            // ID�ظ��������߼�
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

        //�������򣬱�֤id��������
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
            // ID�ظ��������߼�
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

            //�������򣬱�֤id��������
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
    // ������յ���CAN��Ϣ
    int bus = CAN_RegisterDriver(hfdcan);

    uint32_t id = rxhead->Identifier;
    if(rxhead->IdType == FDCAN_STANDARD_ID)
    {
        // �����׼֡
        int idx = binarySearch(can_driver_ids_std[bus], can_driver_count_std[bus], id);
        if (idx >= 0)
            can_drivers_std[bus][idx]->CAN_Update(data);
    }
    else
    {
        // ������չ֡
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
        txHeader.IdType = FDCAN_EXTENDED_ID; // ����Ϊ��չ֡
        txHeader.Identifier = can_id;        // ������չ ID
    }
    else
    {
        txHeader.IdType = FDCAN_STANDARD_ID; // ����Ϊ��׼֡
        txHeader.Identifier = can_id;        // ���ñ�׼ ID
    }

    txHeader.TxFrameType = FDCAN_DATA_FRAME; // ����Ϊ����֡
    txHeader.DataLength = FDCAN_DLC_BYTES_8; // ���ݳ���Ϊ 8 �ֽ�
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;   // ��ʹ��ȫ��ʱ���
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;

    //����
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

    HAL_FDCAN_Start(&hfdcan1); // ����FDCAN
    HAL_FDCAN_Start(&hfdcan2);
    HAL_FDCAN_Start(&hfdcan3);
   
    //ȫ����ģʽ
    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

    return HAL_OK;
}

HAL_StatusTypeDef CAN_Driver::CAN_ConfigALL(FDCAN_HandleTypeDef *hfdcan)
{
    FDCAN_FilterTypeDef sFilterConfig = {0};

    // ���ù�����
    sFilterConfig.FilterType = FDCAN_FILTER_MASK; //MASKģʽ
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //����FIFO0

    sFilterConfig.FilterID1 = 0x000; // ��������ID
    sFilterConfig.FilterID2 = 0x000; // ����Ϊ0����ʾ������IDλ

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
        // ��ȡʧ�ܣ�����
        return;
    }

    CAN_Driver::CAN_HandOut(hfdcan, &rxhead, data);
}