/**
 * @file motor_base.cpp
 * @author XieFField
 * @brief 电机基类
 */
#include "motor_base.h"

/*==================================================*/
DJI_Motor_Base *DJI_Motor_Base::M3508_Instance_CAN1[MAX_DJI_INSTANCE] = {nullptr};
DJI_Motor_Base *DJI_Motor_Base::M3508_Instance_CAN2[MAX_DJI_INSTANCE] = {nullptr};
DJI_Motor_Base *DJI_Motor_Base::M3508_Instance_CAN3[MAX_DJI_INSTANCE] = {nullptr};

uint8_t DJI_Motor_Base::M3508_InstanceCout_CAN1 = 0;
uint8_t DJI_Motor_Base::M3508_InstanceCout_CAN2 = 0;
uint8_t DJI_Motor_Base::M3508_InstanceCout_CAN3 = 0;
/*==================================================*/
DJI_Motor_Base *DJI_Motor_Base::M2006_Instance_CAN1[MAX_DJI_INSTANCE] = {nullptr};
DJI_Motor_Base *DJI_Motor_Base::M2006_Instance_CAN2[MAX_DJI_INSTANCE] = {nullptr};
DJI_Motor_Base *DJI_Motor_Base::M2006_Instance_CAN3[MAX_DJI_INSTANCE] = {nullptr};

uint8_t DJI_Motor_Base::M2006_InstanceCout_CAN1 = 0;
uint8_t DJI_Motor_Base::M2006_InstanceCout_CAN2 = 0;
uint8_t DJI_Motor_Base::M2006_InstanceCout_CAN3 = 0;

/*==================================================*/
DJI_Motor_Base *DJI_Motor_Base::M6020_Instance_CAN1[MAX_DJI_INSTANCE] = {nullptr};
DJI_Motor_Base *DJI_Motor_Base::M6020_Instance_CAN2[MAX_DJI_INSTANCE] = {nullptr};
DJI_Motor_Base *DJI_Motor_Base::M6020_Instance_CAN3[MAX_DJI_INSTANCE] = {nullptr};

uint8_t DJI_Motor_Base::M6020_InstanceCout_CAN1 = 0;
uint8_t DJI_Motor_Base::M6020_InstanceCout_CAN2 = 0;
uint8_t DJI_Motor_Base::M6020_InstanceCout_CAN3 = 0;


DJI_Motor_Base::DJI_Motor_Base(uint32_t can_id, FDCAN_HandleTypeDef *hfdcan, float max_RealCurrent_, int16_t max_virtualCurrent_, uint16_t max_virtualAngle_, DJI_MOTOR_FLAG dji_motor_type_) 
:max_realCurrent(max_RealCurrent_), max_virtualCurrent(max_virtualCurrent_), max_virtualAngle(max_virtualAngle_), DJI_MOTOR_TYPE(dji_motor_type_)
{
    switch(DJI_MOTOR_TYPE)
    {
        case M3508:
            if(hfdcan == &hfdcan1)
            {
                if(can_id > DJI_ID_1 && can_id < DJI_ID_8)
                {
                    M3508_Instance_CAN1[can_id - DJI_ID_1] = this;
                    M3508_InstanceCout_CAN1++;
                }
                else
                    M3508_CAN1ID_ERROR();
            }
            else if(hfdcan == &hfdcan2)
            {
                if(can_id > DJI_ID_1 && can_id < DJI_ID_8)
                {
                    M3508_Instance_CAN2[can_id - DJI_ID_1] = this;
                    M3508_InstanceCout_CAN2++;
                }
                else
                    M3508_CAN2ID_ERROR();
            }
            else if(hfdcan == &hfdcan3)
            {
                if(can_id > DJI_ID_1 && can_id < DJI_ID_8)
                {
                    M3508_Instance_CAN3[can_id - DJI_ID_1] = this;
                    M3508_InstanceCout_CAN3++;
                }
                else
                    M3508_CAN3ID_ERROR();
            }
            break;

        case M2006:
            if(hfdcan == &hfdcan1)
            {
                if(can_id > DJI_ID_1 && can_id < DJI_ID_8)
                {
                    M2006_Instance_CAN1[can_id - DJI_ID_1] = this;
                    M2006_InstanceCout_CAN1++;
                }
                else
                    M2006_CAN1ID_ERROR();
            }
            else if(hfdcan == &hfdcan2)
            {
                if(can_id > DJI_ID_1 && can_id < DJI_ID_8)
                {
                    M2006_Instance_CAN2[can_id - DJI_ID_1] = this;
                    M2006_InstanceCout_CAN2++;
                }
                else
                    M2006_CAN2ID_ERROR();
            }
            else if(hfdcan == &hfdcan3)
            {
                if(can_id > DJI_ID_1 && can_id < DJI_ID_8)
                {
                    M2006_Instance_CAN3[can_id - DJI_ID_1] = this;
                    M2006_InstanceCout_CAN3++;
                }
                else
                    M2006_CAN3ID_ERROR();
            }
            break;

        case M6020:
            if(hfdcan == &hfdcan1)
            {
                if(can_id > DJI_ID_1 && can_id < DJI_ID_8)
                {
                    M6020_Instance_CAN1[can_id - DJI_ID_1] = this;
                    M6020_InstanceCout_CAN1++;
                }
                else
                    M6020_CAN1ID_ERROR();
            }
            else if(hfdcan == &hfdcan2)
            {
                if(can_id > DJI_ID_1 && can_id < DJI_ID_8)
                {
                    M6020_Instance_CAN2[can_id - DJI_ID_1] = this;
                    M6020_InstanceCout_CAN2++;
                }
                else
                    M6020_CAN2ID_ERROR();
            }
            else if(hfdcan == &hfdcan3)
            {
                if(can_id > DJI_ID_1 && can_id < DJI_ID_8)
                {
                    M6020_Instance_CAN3[can_id - DJI_ID_1] = this;
                    M6020_InstanceCout_CAN3++;
                }
                else
                    M6020_CAN3ID_ERROR();
            }
            break;

        default :
            NONE_TYPE_ERROR();
            break;
    }

}

void DJI_Motor_Base::M3508_CAN1ID_ERROR(void)
{
    while(1){}
    //ID非法
}

void DJI_Motor_Base::M3508_CAN2ID_ERROR(void)
{
    while(1){}
    //ID非法
}

void DJI_Motor_Base::M3508_CAN3ID_ERROR(void)
{
    while(1){}
    //ID非法
}

void DJI_Motor_Base::M2006_CAN1ID_ERROR(void)
{
    while(1){}
    //ID非法
}

void DJI_Motor_Base::M2006_CAN2ID_ERROR(void)
{
    while(1){}
    //ID非法
}

void DJI_Motor_Base::M2006_CAN3ID_ERROR(void)
{
    while(1){}
    //ID非法
}

void DJI_Motor_Base::M6020_CAN1ID_ERROR(void)
{
    while(1){}
    //ID非法
}

void DJI_Motor_Base::M6020_CAN2ID_ERROR(void)
{
    while(1){}
    //ID非法
}

void DJI_Motor_Base::M6020_CAN3ID_ERROR(void)
{
    while(1){}
    //ID非法
}   

void DJI_Motor_Base::NONE_TYPE_ERROR(void)
{
    while(1){}
    //电机类型非法
}