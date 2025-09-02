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
    VESC_MOTOR,  //本杰明
    DM_MOTOR,    //达妙
    GO_MOTOR,    //GO关机
    NONE_MOTOR,
} MOTOR_FLAG;

typedef enum DJI_MOTOR_FLAG
{
    M3508_2006,
    M6020,
    NONE_DJI,
}DJI_MOTOR_FLAG;

//限幅函数
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
 * @brief 电机基类，包含电机的所有通用函数
 *        所有电机类都应当继承此类，保证接口统一
 *        原本是可以延顺杨哥的框架，但他那套不是很能兼容GO电机
 *        主要是因为电机基类的关系,所以魔改了
 *
 *        此基类中计算角度和距离的成员函数并非普遍适用，具体实现需根据电机类型进行重写
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

    


    //位置
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

    //速度
    virtual float get_speed() = 0;

    //校准
    virtual void relocate_distance(float distance_) {};      // 重新定位距离
    virtual void relocate_pos(float angle) {};              // 重新定位角度
    virtual uint16_t set_encoder_offset(uint16_t input_value){return this->encoder;};


    //电机分类
    MOTOR_FLAG MOTOR_TYPE = NONE_MOTOR;

    //温度
    virtual uint8_t get_temperature() const { return 0; } 

    //力矩
    virtual float get_torque() {} 

    //减速比
    virtual float get_description() const{return 1.0f;};
};

#define MAX_DJI_INSTANCE 8

/**
 * @brief 大疆电机基类
 * 
 *        所有大疆电机都必须继承此基类
 * 
 *        大疆电机控制分为两帧发送，上帧的数据帧中包含前4个电机的电流，
 *        下帧的数据帧中包含后4个电机的电流。若每次发送时不是把每个电机
 *        电流都赋值进去，那么下一帧来时会覆盖上一帧的数据，从而导致电机
 *        控制异常，前面CAN驱动使用全局变量也是为这个包的饺子。
 *        
 *        注： M6020不可以和M2006、3508处于同一路CAN总线上。
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

    float Out = 0.0f; //输出

    //数据帧生成
    virtual int16_t motor_process() = 0;

    //CAN帧拼接
    void processCANData_DJI();

    //物理量转换接口
    float get_RealCurrent(int16_t virtual_current);
    int16_t get_VirtualCurrent(float real_current);

    //各电机实例表
    static DJI_Motor_Base *M3508_2006_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M3508_2006_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M3508_2006_Instance_CAN3[MAX_DJI_INSTANCE];

    static DJI_Motor_Base *M6020_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M6020_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M6020_Instance_CAN3[MAX_DJI_INSTANCE];

    //各电机CAN总线
    static uint8_t M3508_2006_InstanceCout_CAN1;
    static uint8_t M3508_2006_InstanceCout_CAN2;
    static uint8_t M3508_2006_InstanceCout_CAN3;


    static uint8_t M6020_InstanceCout_CAN1;
    static uint8_t M6020_InstanceCout_CAN2;
    static uint8_t M6020_InstanceCout_CAN3;

private:
    //报错函数
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