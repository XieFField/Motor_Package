/**
 * @file SpeedPlanner.cpp
 * @author XieFField
 * @brief 速度规划
 *        传统的速度规划在末端是锁不住点的，没法实现精确控制
 *        挑战赛时候我尝试过把速度规划和PID结合，即末端采用
 *        PID控制，效果还不赖，所以我打算直接给封成一起的。
 */
#include "SpeedPlanner.h"

