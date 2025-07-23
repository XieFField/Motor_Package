/**
 * @file drive_can.cpp
 * @author XieFField(3432674139@qq.com)
 * 
 * @version 0.1
 * @brief CAN封装 这版会不会用上呢，天知道
 * 
 */
#include "drive_can.h"

void CANDriver::Init()
{
    ConfigureFilter();

    assert_param(hcan_ != NULL);
	
    if (HAL_CAN_Start(hcan_) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

    if (HAL_CAN_ActivateNotification(hcan_, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}
	
	if (HAL_CAN_ActivateNotification(hcan_, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

    if (HAL_CAN_ActivateNotification(hcan_, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

}

void CANDriver::ConfigureFilter(uint32_t id = 0x000, uint32_t mask = 0x000)
{

}

