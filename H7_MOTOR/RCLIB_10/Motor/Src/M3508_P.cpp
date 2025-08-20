/**
 * @file M3508_P19.cpp
 * @author XieFField
 * @brief M3508电机驱动文件
 * 
 */
#include "M3508_P.h"


M3508_P::M3508_P(uint32_t can_id, FDCAN_HandleTypeDef *hfdcan, float motor_description)
    : DJI_Motor_Base(can_id, hfdcan, M3508_MAX_CURRENT, M3508_CURRENT_VIRTUAL, DJI_MOTOR_FLAG::M3508_2006)
    , CAN_Driver(hfdcan, CAN_FrameType::CAN_FRAME_STD, can_id)
{
    
}

void M3508_P::CAN_Update(uint8_t RX_Data[8])
{
    distance = get_distance(RX_Data);
    angle = get_pos(RX_Data);
    rpm = (float)(RX_Data[2] << 8 | RX_Data[3]);
    this->torque = (float)((int16_t)(RX_Data[4] << 8 | RX_Data[5]));
    temperature = RX_Data[6];
}



float M3508_P::get_torque() 
{
    return this->torque;
}

int16_t M3508_P::motor_process()
{
    motor_constraint(&(this->Out), -M3508_MAX_CURRENT, M3508_MAX_CURRENT);
    return get_VirtualCurrent(Out);
}