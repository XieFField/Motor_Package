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
    STD_ID,  // 标准帧（RM电机专用）
    EXT_ID   // 扩展帧（其他电机）
};

// CAN 过滤器参数结构体
struct CanFilterParam 
{
    uint8_t bank;          // 滤波器组（0-27）
    uint8_t fifo;          // 绑定FIFO（0或1）
    CanFrameType frameType;// 帧类型（标准/扩展）
    uint32_t id;           // 过滤ID
    uint32_t maskId;       // 掩码ID
};

// CAN 接收数据结构体
struct CanRxMsg {
    uint32_t id;           // 标准帧ID/扩展帧ID
    CanFrameType frameType;// 帧类型
    uint8_t data[8];       // 数据
    uint8_t len;           // 数据长度
};

// CAN 发送数据结构体
struct CanTxMsg {
    uint32_t id;           // 标准帧ID（11位）/扩展帧ID（29位）
    CanFrameType frameType;// 帧类型
    uint8_t data[8];       // 数据
    uint8_t len;           // 数据长度（1-8）
};

class CANDriver{
public:
    CANDriver(CAN_HandleTypeDef* hcan){hcan_ = hcan;}
    virtual ~CANDriver() = default;

    virtual void Init(){}

    // 使用结构体进行封装发送
    HAL_StatusTypeDef Send(const CanTxMsg& msg);

    // 中断回调入口
    void OnRxCallback(const CAN_RxHeaderTypeDef& rxHeader, uint8_t* data);

    CAN_HandleTypeDef* GetHandle() const { return hcan_; }

protected:
    CAN_HandleTypeDef* hcan_;

    //CAN解析，子类实现
    virtual void HandleReceivedFrame(const CanRxMsg& msg) = 0;

    //滤波掩码，子类实现
    virtual void ConfigureFilter(uint32_t id = 0x000, uint32_t mask = 0x000);

};


#endif

#endif