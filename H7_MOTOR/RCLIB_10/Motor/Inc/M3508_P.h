#ifndef M3508_P_H
#define M3508_P_H

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

#define M3508_KT 0.01562f // 3508����Ȧת�س��� ��λ��N.M/A
#define M3508_DESCRIPTION 19.2032f  // 3508�ļ��ٱ�

#define M3508_MAX_CURRENT 20000.0f // mA
#define M3508_CURRENT_VIRTUAL 16384    // ����ӳ��ֵ

#define M3508_MAXT 0.15622f // ��Ȧ���ת��
#define PI 3.14159265f


class M3508_P : public Motor_Base , public DJI_Motor_Base, public CAN_Driver
{
public:
    M3508_P(uint32_t can_id, FDCAN_HandleTypeDef *hfdcan, float motor_description = M3508_DESCRIPTION);

    int16_t motor_process() override;

    virtual float get_description() const{return description;} 

    virtual void CAN_Update(uint8_t RX_Data[8]) override;

    virtual float get_torque() override;

    virtual uint8_t get_temperature() const override {return temperature;};

    float Out = 0.0f; // ������� mA

private:
    float description = M3508_DESCRIPTION;

    float rpm = 0.0f;
    float angle = 0.0f;
    float distance = 0.0f;
    float torque = 0.0f;
    uint8_t temperature = 0;
};


#endif


#endif //M3508_P_H