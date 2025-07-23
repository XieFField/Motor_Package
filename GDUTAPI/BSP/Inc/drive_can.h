#ifndef DRIVE_CAN_H
#define DRIVE_CAN_H

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "can.h"
#include <stdint.h>
#include <stdbool.h>
#include <cassert>

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

enum class CanFrameType 
{
    STD_ID,  // ��׼֡��RM���ר�ã�
    EXT_ID   // ��չ֡�����������
};

// CAN �����������ṹ��
struct CanFilterParam 
{
    uint8_t bank;          // �˲����飨0-27��
    uint8_t fifo;          // ��FIFO��0��1��
    CanFrameType frameType;// ֡���ͣ���׼/��չ��
    uint32_t id;           // ����ID
    uint32_t maskId;       // ����ID
};

// CAN �������ݽṹ��
struct CanRxMsg {
    uint32_t id;           // ��׼֡ID/��չ֡ID
    CanFrameType frameType;// ֡����
    uint8_t data[8];       // ����
    uint8_t len;           // ���ݳ���
};

// CAN �������ݽṹ��
struct CanTxMsg {
    uint32_t id;           // ��׼֡ID��11λ��/��չ֡ID��29λ��
    CanFrameType frameType;// ֡����
    uint8_t data[8];       // ����
    uint8_t len;           // ���ݳ��ȣ�1-8��
};

class CANDriver{
public:
    CANDriver(CAN_HandleTypeDef* hcan){hcan_ = hcan;}
    virtual ~CANDriver() = default;

    virtual void Init(){}

    // ʹ�ýṹ����з�װ����
    HAL_StatusTypeDef Send(const CanTxMsg& msg);

    // �жϻص����
    void OnRxCallback(const CAN_RxHeaderTypeDef& rxHeader, uint8_t* data);

    CAN_HandleTypeDef* GetHandle() const { return hcan_; }

protected:
    CAN_HandleTypeDef* hcan_;

    //CAN����������ʵ��
    virtual void HandleReceivedFrame(const CanRxMsg& msg) = 0;

    //�˲����룬����ʵ��
    virtual void ConfigureFilter(uint32_t id = 0x000, uint32_t mask = 0x000);

};


#endif

#endif