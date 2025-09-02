#ifndef GO1_M8010_H
#define GO1_M8010_H

#pragma once

#include "motor_base.h"
#include "PID.h"
#include "SpeedPlanner.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}

#endif

#ifdef __cplusplus

class GO1_M8010 : public Motor_Base, public CAN_Driver
{
public:
    GO1_M8010();
    void CAN_update(uint8_t RX_Data[8]) {}; //∂‘EXT≤ª  ”√

    void CAN_update_EXT(uint32_t can_id, uint8_t RX_Data[8]);
private:

};

#endif


#endif //GO1_M8010_H