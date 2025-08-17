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
    M2006,
    M3508,
    M6020,
    NONE_DJI,
}DJI_MOTOR_FLAG;

/**
 * @brief 电机基类，包含电机的所有通用函数
 *        所有电机类都应当继承此类，保证接口统一
 */
class Motor_Base{
public:
    Motor_Base();
    //速度
    virtual float get_speed() = 0; 
    virtual void  set_speed(float target_speed);

    virtual void  send_speed(float target_speed) {};

    //位置
    virtual float get_pos(){};
    virtual float get_distance(){};
    virtual void  set_pos(float target_pos){};
    virtual void  set_distance(float target_distance){};

    virtual void set_current(float target_current){};//设置电流

    //校准
    virtual void relocate_distance(float distance) {};   // 重新定位距离
    virtual void relocate_pos(float angle) {}; // 重新定位角度

    //获取目标值
    virtual float get_target_speed(){};
    virtual float get_target_pos(){};
    virtual float get_target_distance(){};
    virtual float get_target_current(){};

    //速度规划
    virtual bool distance_speedPlan(float target_distance, float max_speed, float max_accel, float max_decel, float final_speed){};
    virtual bool position_speedPlan(float target_distance, float max_speed, float max_accel, float max_decel, float final_speed){};

    //速度规划重启
    virtual void distance_speedPlanReStart(){};
    virtual void position_speedPlanReStart(){};

    //电机分类
    MOTOR_FLAG MOTOR_TYPE = NONE_MOTOR;
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

    //数据帧生成
    virtual int16_t motor_process() = 0;

    //CAN帧拼接
    void processCANData_DJI();

    //物理量转换接口
    float get_RealCurrent(int16_t virtual_current);
    float get_VirtualCurrent(float real_current);
    float get_RealAngle(uint32_t virtual_angle);

    float get_RealDistance();

    //各电机实例表
    static DJI_Motor_Base *M3508_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M3508_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M3508_Instance_CAN3[MAX_DJI_INSTANCE];

    static DJI_Motor_Base *M2006_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M2006_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M2006_Instance_CAN3[MAX_DJI_INSTANCE];

    static DJI_Motor_Base *M6020_Instance_CAN1[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M6020_Instance_CAN2[MAX_DJI_INSTANCE];
    static DJI_Motor_Base *M6020_Instance_CAN3[MAX_DJI_INSTANCE];

    //各电机CAN总线
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
    //报错函数
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