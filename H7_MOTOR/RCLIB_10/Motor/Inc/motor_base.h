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
    M3508_2006,
    M6020,
    NONE_DJI,
}DJI_MOTOR_FLAG;

//�޷�����
template <typename T>
void motor_constraint(T *val, T min, T max)
{
    if (*val > max)
    {
        *val = max;
    }
    else if (*val < min)
    {
        *val = min;
    }
}

/**
 * @brief ������࣬�������������ͨ�ú���
 *        ���е���඼Ӧ���̳д��࣬��֤�ӿ�ͳһ
 *        ԭ���ǿ�����˳���Ŀ�ܣ��������ײ��Ǻ��ܼ���GO���
 *        ��Ҫ����Ϊ�������Ĺ�ϵ,����ħ����
 *
 *        �˻����м���ǶȺ;���ĳ�Ա���������ձ����ã�����ʵ������ݵ�����ͽ�����д
 */
class Motor_Base{
private:
    int round_cnt = 0;
    virtual int16_t encoder_max() const { return 8192; }
    virtual float encoder_angle_ratio() const { return 8192.0f / 360.0f; }

    
protected:
    uint16_t encoder = 0;
    float angle = 0, encoder_last = 0;
    bool encoder_is_init = false;
    float encoder_offset = 0;
public:
    Motor_Base(){};

    


    //λ��
    virtual float get_pos(uint8_t can_Rx_Data[8])
    {
        uint16_t virtual_angle = (uint16_t)(can_Rx_Data[0] << 8 | can_Rx_Data[1]);
        return virtual_angle / encoder_angle_ratio();
    }

    virtual float get_distance(uint8_t can_Rx_Data[8])
    {
        encoder = (uint16_t)(can_Rx_Data[0] << 8 | can_Rx_Data[1]);
        if(encoder_is_init)
        {
            if(this->encoder - this->encoder_last < encoder_max() / -2)
                this->round_cnt++;
            else if(this->encoder - this->encoder_last > encoder_max() / 2)
                this->round_cnt--;
        }
        else
        {
            encoder_offset = encoder;
            encoder_is_init = true;
        }
        this->encoder_last = this->encoder;
        int32_t total_encoder = round_cnt * 8192 + encoder - encoder_offset;
        return total_encoder / encoder_angle_ratio() / get_description();
    }

    //�ٶ�
    virtual float get_speed() = 0;

    //У׼
    virtual void relocate_distance(float distance_) {};      // ���¶�λ����
    virtual void relocate_pos(float angle) {};              // ���¶�λ�Ƕ�
    virtual uint16_t set_encoder_offset(uint16_t input_value){return this->encoder;};


    //�������
    MOTOR_FLAG MOTOR_TYPE = NONE_MOTOR;

    //�¶�
    virtual uint8_t get_temperature() const { return 0; } 

    //����
    virtual float get_torque() {} 

    //���ٱ�
    virtual float get_description() const{return 1.0f;};
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
 *        ע�� M6020�����Ժ�M2006��3508����ͬһ·CAN�����ϡ�
 */
class DJI_Motor_Base
{
public:
    float realCurrent = 0.0f;

    int16_t target_virtualCurrent = 0;

    float max_realCurrent = 0.0f;

    int16_t max_virtualCurrent = 0;

    DJI_MOTOR_FLAG DJI_MOTOR_TYPE = NONE_DJI;
    DJI_Motor_Base(uint32_t can_id, FDCAN_HandleTypeDef *hfdcan, 
                   float max_RealCurrent_, int16_t max_virtualCurrent_,
                    DJI_MOTOR_FLAG dji_motor_type_);

    float Out = 0.0f; //���

    //����֡����
    virtual int16_t motor_process() = 0;

    //CAN֡ƴ��
    void processCANData_DJI();

    //������ת���ӿ�
    float get_RealCurrent(int16_t virtual_current);
    int16_t get_VirtualCurrent(float real_current);

    //�����ʵ����
    static DJI_Motor_Base *M3508_2006_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M3508_2006_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M3508_2006_Instance_CAN3[MAX_DJI_INSTANCE];

    static DJI_Motor_Base *M6020_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M6020_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M6020_Instance_CAN3[MAX_DJI_INSTANCE];

    //�����CAN����
    static uint8_t M3508_2006_InstanceCout_CAN1;
    static uint8_t M3508_2006_InstanceCout_CAN2;
    static uint8_t M3508_2006_InstanceCout_CAN3;


    static uint8_t M6020_InstanceCout_CAN1;
    static uint8_t M6020_InstanceCout_CAN2;
    static uint8_t M6020_InstanceCout_CAN3;

private:
    //������
    void M3508_2006_CAN1ID_ERROR(void);
    void M3508_2006_CAN2ID_ERROR(void);
    void M3508_2006_CAN3ID_ERROR(void);


    void M6020_CAN1ID_ERROR(void);
    void M6020_CAN2ID_ERROR(void);
    void M6020_CAN3ID_ERROR(void);

    void NONE_TYPE_ERROR(void);

    uint32_t send_idLOW(){return 0x200;}
    uint32_t send_idHIGH(){return 0x1FF;}

    uint32_t send_idLOW_GM6020(){return 0x1FE;}
    uint32_t send_idHigh_GM6020(){return 0x2FE;}
};

#endif

#endif // MOTOR_BASE_H