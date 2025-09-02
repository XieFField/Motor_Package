#ifndef GM6020_H
#define GM6020_H

#pragma once

#include "motor_base.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}

#endif

#define GM6020_MAX_CURRENT 3000.0f
#define GM6020_CURRENT_VIRTUAL 16384

#ifdef __cplusplus

class GM6020 : public Motor_Base, public CAN_Driver, public DJI_Motor_Base
{
public:
    GM6020(uint8_t can_id, FDCAN_HandleTypeDef *hfdcan);

    virtual void CAN_Update(uint8_t RX_Data[8]) override;

    virtual int16_t motor_process() override;

    virtual uint16_t set_encoder_offset(uint16_t input_value) override
    {
        this->encoder_offset = input_value;
        this->encoder_last = input_value;
        this->encoder_is_init = true;
        return this->encoder;
    }

    virtual float get_torque() override;

    virtual uint8_t get_temperature() const override { return temperature; }

    float Out = 0.0f;   

private:

    float rpm = 0.0f;
    float angle = 0.0f;
    float distance = 0.0f;
    float torque = 0.0f;
    uint8_t temperature = 0;
};

#endif


#endif //GM6020_H