#ifndef VESC_H
#define VESC_H

#pragma once

#include "motor_base.h"
#include "fdcan.h"
#include "math.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

typedef enum VESC_MODE{
    SET_CURRENT,    //電流
    SET_eRPM,       //電氣轉速
    SET_DUTY,      //佔空比
    SET_BRAKE,     //煞車
    SET_POS,       //位置
    SET_NULL      //空
} VESC_MODE;


class VESC : public Motor_Base , public CAN_Driver
{
public:
    VESC(uint32_t can_id_, FDCAN_HandleTypeDef *hfdcan_, uint8_t motor_poles_, uint8_t motor_direction);
    ~VESC();
    VESC_MODE mode = SET_eRPM;

    void CAN_Update(uint8_t Rx_Data[8]);
    void send_Data();

    uint32_t get_ext_id() const { return ext_id; }

    uint8_t get_motor_poles() const { return motor_poles; }
    virtual float get_description()  const override { return motor_direction; } 

    int32_t Out = 0;



    virtual float get_speed() override {return (float)RPM;}
    int32_t get_eRPM() const {return eRPM;}
    int16_t get_current() const {return current;}

private:
    int16_t current;
    int32_t RPM, eRPM;

    uint32_t ext_id = 0;
    uint8_t motor_poles = 1;
    uint8_t motor_direction = 1;
    void set_eRPM();
    void set_Duty();
    void set_Brake();
    void set_Pos();
    void set_Current();
};

//VESC驱动器的can命令枚举
typedef enum {
	VESC_PACKET_SET_DUTY						= 0,
	VESC_PACKET_SET_CURRENT					    = 1,
	VESC_PACKET_SET_CURRENT_BRAKE			    = 2,
	VESC_PACKET_SET_RPM						    = 3,
	VESC_PACKET_SET_POS						    = 4,
	VESC_PACKET_FILL_RX_BUFFER				    = 5,
	VESC_PACKET_FILL_RX_BUFFER_LONG			    = 6,
	VESC_PACKET_PROCESS_RX_BUFFER			    = 7,
	VESC_PACKET_PROCESS_SHORT_BUFFER			= 8,
	VESC_PACKET_STATUS						    = 9,
	VESC_PACKET_SET_CURRENT_REL				    = 10,
	VESC_PACKET_SET_CURRENT_BRAKE_REL		    = 11,
	VESC_PACKET_SET_CURRENT_HANDBRAKE		    = 12,
	VESC_PACKET_SET_CURRENT_HANDBRAKE_REL	    = 13,
	CAN_PACKET_STATUS_2						    = 14,
	CAN_PACKET_STATUS_3						    = 15,
	VESC_PACKET_STATUS_4						= 16,
	VESC_PACKET_PING							= 17,
	VESC_PACKET_PONG							= 18,
	VESC_PACKET_DETECT_APPLY_ALL_FOC			= 19,
	VESC_PACKET_DETECT_APPLY_ALL_FOC_RES		= 20,
	VESC_PACKET_CONF_CURRENT_LIMITS			    = 21,
	VESC_PACKET_CONF_STORE_CURRENT_LIMITS	    = 22,
	VESC_PACKET_CONF_CURRENT_LIMITS_IN		    = 23,
	VESC_PACKET_CONF_STORE_CURRENT_LIMITS_IN	= 24,
	VESC_PACKET_CONF_FOC_ERPMS				    = 25,
	VESC_PACKET_CONF_STORE_FOC_ERPMS		    = 26,
	VESC_PACKET_STATUS_5					    = 27,
	VESC_PACKET_POLL_TS5700N8501_STATUS		    = 28,
	VESC_PACKET_CONF_BATTERY_CUT				= 29,
	VESC_PACKET_CONF_STORE_BATTERY_CUT		    = 30,
	VESC_PACKET_SHUTDOWN						= 31,
	VESC_PACKET_IO_BOARD_ADC_1_TO_4			    = 32,
	VESC_PACKET_IO_BOARD_ADC_5_TO_8			    = 33,
	VESC_PACKET_IO_BOARD_ADC_9_TO_12		    = 34,
	VESC_PACKET_IO_BOARD_DIGITAL_IN			    = 35,
	VESC_PACKET_IO_BOARD_SET_OUTPUT_DIGITAL	    = 36,
	VESC_PACKET_IO_BOARD_SET_OUTPUT_PWM		    = 37,
	VESC_PACKET_BMS_V_TOT						= 38,
	VESC_PACKET_BMS_I							= 39,
	VESC_PACKET_BMS_AH_WH						= 40,
	VESC_PACKET_BMS_V_CELL						= 41,
	VESC_PACKET_BMS_BAL							= 42,
	VESC_PACKET_BMS_TEMPS						= 43,
	VESC_PACKET_BMS_HUM							= 44,
	VESC_PACKET_BMS_SOC_SOH_TEMP_STAT			= 45,
	VESC_PACKET_PSW_STAT						= 46,
	VESC_PACKET_PSW_SWITCH						= 47,
	VESC_PACKET_BMS_HW_DATA_1					= 48,
	VESC_PACKET_BMS_HW_DATA_2					= 49,
	VESC_PACKET_BMS_HW_DATA_3					= 50,
	VESC_PACKET_BMS_HW_DATA_4				    = 51,
	VESC_PACKET_BMS_HW_DATA_5				    = 52,
	VESC_PACKET_BMS_AH_WH_CHG_TOTAL			    = 53,
	VESC_PACKET_BMS_AH_WH_DIS_TOTAL			    = 54,
	VESC_PACKET_UPDATE_PID_POS_OFFSET		    = 55,
	VESC_PACKET_POLL_ROTOR_POS				    = 56,
	VESC_PACKET_NOTIFY_BOOT					    = 57,
	VESC_PACKET_STATUS_6						= 58,
	VESC_PACKET_GNSS_TIME					    = 59,
	VESC_PACKET_GNSS_LAT						= 60,
	VESC_PACKET_GNSS_LON						= 61,
	VESC_PACKET_GNSS_ALT_SPEED_HDOP			    = 62
}
VESC_PACKET_ID;

#endif // VESC_H