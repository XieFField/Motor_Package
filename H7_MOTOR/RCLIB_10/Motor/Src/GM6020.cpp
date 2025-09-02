
/**
 * @file GM6020.cpp
 * @author XieFField
 * @brief GM6020????
 * @date 2025-8-18
 */
#include "GM6020.h"


GM6020::GM6020(uint8_t can_id, FDCAN_HandleTypeDef *hfdcan)
    : DJI_Motor_Base(can_id, hfdcan, GM6020_MAX_CURRENT, GM6020_CURRENT_VIRTUAL, DJI_MOTOR_FLAG::M6020)
    , CAN_Driver(hfdcan, CAN_FrameType::CAN_FRAME_STD, can_id)
{

}

void GM6020::CAN_Update(uint8_t RX_Data[8])
{
    // ??CAN??
    angle = get_pos(RX_Data);
    distance = get_distance(RX_Data);
    rpm = (float)(RX_Data[2] << 8 | RX_Data[3]);
    this->torque = (float)((int16_t)(RX_Data[4] << 8 | RX_Data[5]));
    this->temperature = RX_Data[6];
}

float GM6020::get_torque() 
{
    return (float)this->torque;
}

int16_t GM6020::motor_process()
{
    motor_constraint(&(this->Out), -GM6020_MAX_CURRENT, GM6020_MAX_CURRENT);
    return get_VirtualCurrent(Out);
}