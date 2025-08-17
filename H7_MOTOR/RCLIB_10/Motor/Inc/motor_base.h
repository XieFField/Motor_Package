#ifndef MOTOR_BASE_H
#define MOTOR_BASE_H

#pragma once

#include "fdCAN_driver.h"
#include "tool.h"

#ifdef __cplusplus

#define DJI_ID_1 0x201
#define DJI_ID_2 0x202
#define DJI_ID_3 0x203
#define DJI_ID_4 0x204

#define DJI_ID_5 0x205
#define DJI_ID_6 0x206
#define DJI_ID_7 0x207
#define DJI_ID_8 0x208

typedef enum MOTOR_FLAG
{
    DJI_MOTOR,   //DJI
    VESC_MOTOR,  //������
    DM_MOTOR,    //����
    GO_MOTOR,    //GO�ػ�
    NONE_MOTOR,
} MOTOR_FLAG;

typedef enum DJI_MOTOR_FLAG
{
    M2006,
    M3508,
    M6020,
    NONE_DJI,
}DJI_MOTOR_FLAG;

/**
 * @brief ������࣬�������������ͨ�ú���
 *        ���е���඼Ӧ���̳д��࣬��֤�ӿ�ͳһ
 */
class Motor_Base{
public:
    Motor_Base();
    //�ٶ�
    virtual float get_speed() = 0; 
    virtual void  set_speed(float target_speed);

    virtual void  send_speed(float target_speed) {};

    //λ��
    virtual float get_pos(){};
    virtual float get_distance(){};
    virtual void  set_pos(float target_pos){};
    virtual void  set_distance(float target_distance){};

    virtual void set_current(float target_current){};//���õ���

    //У׼
    virtual void relocate_distance(float distance) {};   // ���¶�λ����
    virtual void relocate_pos(float angle) {}; // ���¶�λ�Ƕ�

    //��ȡĿ��ֵ
    virtual float get_target_speed(){};
    virtual float get_target_pos(){};
    virtual float get_target_distance(){};
    virtual float get_target_current(){};

    //�ٶȹ滮
    virtual bool distance_speedPlan(float target_distance, float max_speed, float max_accel, float max_decel, float final_speed){};
    virtual bool position_speedPlan(float target_distance, float max_speed, float max_accel, float max_decel, float final_speed){};

    //�ٶȹ滮����
    virtual void distance_speedPlanReStart(){};
    virtual void position_speedPlanReStart(){};

    //�������
    MOTOR_FLAG MOTOR_TYPE = NONE_MOTOR;
};

#define MAX_DJI_INSTANCE 8

/**
 * @brief �󽮵������
 * 
 *        ���д󽮵��������̳д˻���
 * 
 *        �󽮵�����Ʒ�Ϊ��֡���ͣ���֡������֡�а���ǰ4������ĵ�����
 *        ��֡������֡�а�����4������ĵ�������ÿ�η���ʱ���ǰ�ÿ�����
 *        ��������ֵ��ȥ����ô��һ֡��ʱ�Ḳ����һ֡�����ݣ��Ӷ����µ��
 *        �����쳣��ǰ��CAN����ʹ��ȫ�ֱ���Ҳ��Ϊ������Ľ��ӡ�
 *        
 */
class DJI_Motor_Base
{
public:

    float realAngle = 0.0f;
    float realCurrent = 0.0f;
    float realRPM = 0.0f;
    int16_t target_virtualCurrent = 0;

    float max_realCurrent = 0.0f;
    uint16_t max_virtualAngle = 0.0f;
    int16_t max_virtualCurrent = 0;

    DJI_MOTOR_FLAG DJI_MOTOR_TYPE = NONE_DJI;
    DJI_Motor_Base(uint32_t can_id, FDCAN_HandleTypeDef *hfdcan, 
                   float max_RealCurrent_, int16_t max_virtualCurrent_, uint16_t max_virtualAngle_, 
                    DJI_MOTOR_FLAG dji_motor_type_);

    //����֡����
    virtual int16_t motor_process() = 0;

    //CAN֡ƴ��
    void processCANData_DJI();

    //������ת���ӿ�
    float get_RealCurrent(int16_t virtual_current);
    float get_VirtualCurrent(float real_current);
    float get_RealAngle(uint32_t virtual_angle);

    float get_RealDistance();

    //�����ʵ����
    static DJI_Motor_Base *M3508_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M3508_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M3508_Instance_CAN3[MAX_DJI_INSTANCE];

    static DJI_Motor_Base *M2006_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M2006_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M2006_Instance_CAN3[MAX_DJI_INSTANCE];

    static DJI_Motor_Base *M6020_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M6020_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M6020_Instance_CAN3[MAX_DJI_INSTANCE];

    //�����CAN����
    static uint8_t M3508_InstanceCout_CAN1;
    static uint8_t M3508_InstanceCout_CAN2;
    static uint8_t M3508_InstanceCout_CAN3;

    static uint8_t M2006_InstanceCout_CAN1;
    static uint8_t M2006_InstanceCout_CAN2;
    static uint8_t M2006_InstanceCout_CAN3;

    static uint8_t M6020_InstanceCout_CAN1;
    static uint8_t M6020_InstanceCout_CAN2;
    static uint8_t M6020_InstanceCout_CAN3;

private:
    //������
    void M3508_CAN1ID_ERROR(void);
    void M3508_CAN2ID_ERROR(void);
    void M3508_CAN3ID_ERROR(void);

    void M2006_CAN1ID_ERROR(void);
    void M2006_CAN2ID_ERROR(void);
    void M2006_CAN3ID_ERROR(void);

    void M6020_CAN1ID_ERROR(void);
    void M6020_CAN2ID_ERROR(void);
    void M6020_CAN3ID_ERROR(void);

    void NONE_TYPE_ERROR(void);

};

#endif

#endif // MOTOR_BASE_H