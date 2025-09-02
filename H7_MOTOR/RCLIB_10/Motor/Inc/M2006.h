#ifndef M2006_H
#define M2006_H

#pragma once

#include "motor_base.h"
#include "fdCAN_driver.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}

#endif

#ifdef __cplusplus

#define M2006_DESCRIPTION 36.0f
#define M2006_MAX_CURRENT 10000.0f // mA
#define M2006_CURRENT_VIRTUAL 10000
#define PI 3.14159265f

class M2006 : public Motor_Base , public DJI_Motor_Base, public CAN_Driver
{
public:
    M2006(uint32_t can_id, FDCAN_HandleTypeDef *hfdcan, float motor_description = M2006_DESCRIPTION);

    int16_t motor_process() override;

    virtual float get_description() const{return description;} 

    virtual void CAN_Update(uint8_t RX_Data[8]) override;

    virtual float get_torque() override;

    virtual uint8_t get_temperature() const override {return temperature;};

    virtual float get_speed() override {return rpm;};

    float Out = 0.0f; // Êä³öµçÁ÷ mA

private:
    float description = M2006_DESCRIPTION;
    float rpm = 0.0f;
    float angle = 0.0f;
    float distance = 0.0f;
    float torque = 0.0f;
    uint8_t temperature = 0;
};


#endif


#endif //M2006_H