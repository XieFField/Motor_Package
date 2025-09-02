/**
 * @file VESC.cpp
 * @author XieFField
 * @brief VESC驅動文件，注意：VESC使用的拓展幀，與DJI電調衝突，請勿在同一CAN總線上使用。
 */

#include "VESC.h"

VESC::VESC(uint32_t can_id_, FDCAN_HandleTypeDef *hfdcan_, uint8_t motor_poles_, uint8_t motor_direction_)
    : CAN_Driver(hfdcan_, CAN_FRAME_EXT, can_id_)
{
    this->motor_poles = motor_poles_;
    this->motor_direction = motor_direction_;
    this->ext_id = can_id_;
}

void VESC::send_Data()
{
    switch (mode)
    {
        case SET_CURRENT:
            /* code */
            set_Current();
            break;

        case SET_eRPM:
            /* code */
            set_eRPM();
            break;

        case SET_DUTY:
            /* code */
            set_Duty();
            break;

        case SET_BRAKE:
            /* code */
            set_Brake();
            break;

        case SET_POS:
            /* code */
            set_Pos();
            break;

        default:
            break;
    }
}

void VESC::set_eRPM()
{
    uint8_t tx_buff[8] = {0};
    
    int32_t erpm = Out * motor_poles * motor_direction;

    ext_id = (VESC_PACKET_SET_RPM << 8) | (CAN_ID_ & 0xFF);

    tx_buff[0] = (erpm >> 24) & 0xFF;
    tx_buff[1] = (erpm >> 16) & 0xFF;
    tx_buff[2] = (erpm >> 8) & 0xFF;
    tx_buff[3] = (erpm) & 0xFF;

    CAN_Send(hfdcan_, ext_id, 1, tx_buff);
}

void VESC::set_Duty()
{
    uint8_t tx_buff[8] = {0};

    ext_id = (VESC_PACKET_SET_DUTY << 8) | (CAN_ID_ & 0xFF);

    tx_buff[0] = (Out >> 24) & 0xFF;
    tx_buff[1] = (Out >> 16) & 0xFF;
    tx_buff[2] = (Out >> 8) & 0xFF;
    tx_buff[3] = (Out) & 0xFF;

    CAN_Send(hfdcan_, ext_id, 1, tx_buff);
}

void VESC::set_Brake()
{
    uint8_t tx_buff[8] = {0};

    ext_id = (VESC_PACKET_SET_CURRENT_BRAKE << 8) | (CAN_ID_ & 0xFF);

    tx_buff[0] = (Out >> 24) & 0xFF;
    tx_buff[1] = (Out >> 16) & 0xFF;
    tx_buff[2] = (Out >> 8) & 0xFF;
    tx_buff[3] = (Out) & 0xFF;

    CAN_Send(hfdcan_, ext_id, 1, tx_buff);
}

void VESC::set_Pos()
{
    uint8_t tx_buff[8] = {0};

    ext_id = (VESC_PACKET_SET_POS << 8) | (CAN_ID_ & 0xFF);

    tx_buff[0] = (Out >> 24) & 0xFF;
    tx_buff[1] = (Out >> 16) & 0xFF;
    tx_buff[2] = (Out >> 8) & 0xFF;
    tx_buff[3] = (Out) & 0xFF;

    CAN_Send(hfdcan_, ext_id, 1, tx_buff);
}

void VESC::set_Current()
{
    uint8_t tx_buff[8] = {0};

    ext_id = (VESC_PACKET_SET_CURRENT << 8) | (CAN_ID_ & 0xFF);

    tx_buff[0] = (Out >> 24) & 0xFF;
    tx_buff[1] = (Out >> 16) & 0xFF;
    tx_buff[2] = (Out >> 8) & 0xFF;
    tx_buff[3] = (Out) & 0xFF;

    CAN_Send(hfdcan_, ext_id, 1, tx_buff);
}

void VESC::CAN_Update(uint8_t Rx_Data[8])
{
    current = (int16_t)(Rx_Data[4] << 8 | Rx_Data[5]);
    eRPM = (int32_t)((Rx_Data[0] << 24) | (Rx_Data[1] << 16) | (Rx_Data[2] << 8) | Rx_Data[3]);
    RPM = eRPM / motor_poles / motor_direction;
}

