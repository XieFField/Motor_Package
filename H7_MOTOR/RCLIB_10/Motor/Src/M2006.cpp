/**
 * @file M2006.cpp
 * @author XiFField
 * @brief M2006Çý¶¯ÎÄ¼þ
 */
#include "M2006.h"

M2006::M2006(uint32_t can_id, FDCAN_HandleTypeDef *hfdcan, float motor_description)
    : DJI_Motor_Base(can_id, hfdcan, M2006_MAX_CURRENT, M2006_CURRENT_VIRTUAL, DJI_MOTOR_FLAG::M3508_2006)
    , CAN_Driver(hfdcan, CAN_FrameType::CAN_FRAME_STD, can_id)
{
    
}

void M2006::CAN_Update(uint8_t RX_Data[8])
{
    distance = get_distance(RX_Data);
    angle = get_pos(RX_Data);
    rpm = (float)(RX_Data[2] << 8 | RX_Data[3]);
    this->torque = (int16_t)(RX_Data[4] << 8 | RX_Data[5]);
    temperature = RX_Data[6];
}

float M2006::get_torque() 
{
    return this->torque;
}

int16_t M2006::motor_process()
{
    motor_constraint(&(this->Out), -M2006_MAX_CURRENT, M2006_MAX_CURRENT);
    return get_VirtualCurrent(Out);
}