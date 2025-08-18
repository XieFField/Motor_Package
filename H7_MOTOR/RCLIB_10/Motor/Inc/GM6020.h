#ifndef GM6020_H
#define GM6020_H

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

class GM6020 : public Motor_Base, public CAN_Driver
{
public:
    GM6020();
    void CAN_update(uint8_t RX_Data[8]) {}; //∂‘EXT≤ª  ”√

private:

}

#endif


#endif //GM6020_H