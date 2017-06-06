#ifndef __PIN_DEFINE_H__
#define __PIN_DEFINE_H__
/***********************************************************************/
/*  File Name   :Pin_Define.h                                          */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8L151G6     Crystal: 16M HSI                       */
/*  DESCRIPTION :                                                      */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include "Timer.h"
//以下是IO定义
/********************LED寄存器*****************************************/
extern u8 LED_Cache;
#define LED_ON 1
#define LED_OFF 0
#define LED_YELLOW LED_Cache       //黄灯
#define LED_YELLOW_DDR PC_DDR_DDR0 //黄灯方向
#define LED_YELLOW_CR1 PC_CR1_C10  //黄灯类型
#define LED_YELLOW_CR2 PC_CR2_C20  //黄灯输出频率

#define LED_RED LED_Cache       //红灯
#define LED_RED_DDR PC_DDR_DDR1 //方向
#define LED_RED_CR1 PC_CR1_C11  //类型
#define LED_RED_CR2 PC_CR2_C21  //输出频率

#define Receiver_LED_TX LED_Cache         //PC_ODR_ODR1       // Output   受信机送信指示  高电平有效
#define Receiver_LED_TX_direc PC_DDR_DDR1 // Output   受信机送信指示  高电平有效
#define Receiver_LED_TX_CR1 PC_CR1_C11    // Output   受信机送信指示  高电平有效

#define Receiver_LED_RX PC_ODR_ODR1       //PC_ODR_ODR0       // Output   受信机受信指示  高电平有效
#define Receiver_LED_RX_direc PC_DDR_DDR0 // Output   受信机受信指示  高电平有效
#define Receiver_LED_RX_CR1 PC_CR1_C10    // Output   受信机受信指示  高电平有效

#define Receiver_LED_OUT PC_ODR_ODR0       //PC_ODR_ODR4       // Output   受信机继电器动作输出  高电平有效
#define Receiver_LED_OUT_direc PC_DDR_DDR4 // Output   受信机继电器动作输出  高电平有效
#define Receiver_LED_OUT_CR1 PC_CR1_C14    // Output   受信机继电器动作输出  高电平有效

#define YELLOWLED_FLASH_SECOND() \
    {                            \
        LED_YELLOW = LED_ON;     \
        LedYELLOWTimer = 750;    \
    }
#define YELLOWLED_OFF()       \
    {                         \
        LED_YELLOW = LED_OFF; \
        LedYELLOWTimer = 0;   \
    }
#define YELLOWLED_FLASH()         \
    {                             \
        LED_YELLOW = !LED_YELLOW; \
        LedYELLOWTimer = 60;      \
    }

#define REDLED_FLASH_SECOND() \
    {                         \
        LED_RED = LED_ON;     \
        LedREDTimer = 750;    \
    }
#define REDLED_OFF()       \
    {                      \
        LED_RED = LED_OFF; \
        LedREDTimer = 0;   \
    }
#define REDLED_FLASH()      \
    {                       \
        LED_RED = !LED_RED; \
        LedREDTimer = 60;   \
    }
/******************以下是ADF7030-1寄存器************************************/
/* ADF7030-1 register interface */
#define ADF7030_REST ADF7030_REST_Cache
#define ADF7030_REST_DDR ADF7030_REST_Cache
#define ADF7030_REST_CR1 ADF7030_REST_Cache
#define ADF7030_REST_CR2 ADF7030_REST_Cache

#define ADF7030_GPIO2 PC_IDR_IDR5
#define ADF7030_GPIO2_DDR PC_DDR_DDR5
#define ADF7030_GPIO2_CR1 PC_CR1_C15
#define ADF7030_GPIO2_CR2 PC_CR2_C25

#define ADF7030_GPIO3 PD_IDR_IDR4
#define ADF7030_GPIO3_DDR PD_DDR_DDR4
#define ADF7030_GPIO3_CR1 PD_CR1_C14
#define ADF7030_GPIO3_CR2 PD_CR2_C24
//
#define ADF7030_GPIO4 PC_IDR_IDR4
#define ADF7030_GPIO4_DDR PC_DDR_DDR4
#define ADF7030_GPIO4_CR1 PC_CR1_C14
#define ADF7030_GPIO4_CR2 PC_CR2_C24

#define ADF7030_GPIO5 PC_IDR_IDR6
#define ADF7030_GPIO5_DDR PC_DDR_DDR6
#define ADF7030_GPIO5_CR1 PC_CR1_C16
#define ADF7030_GPIO5_CR2 PC_CR2_C26

#define ADF7030CLK ADF7030_GPIO4
#define ADF7030DATA ADF7030_GPIO5
/******************以下是KEY寄存器*******输入*****************************/
//#define KEY_SW2 PA_IDR_IDR4
//#define KEY_SW2_DDR PA_DDR_DDR4
//#define KEY_SW2_CR1 PA_CR1_C14
//#define KEY_SW2_CR2 PA_CR2_C24
//
//#define KEY_SW3 PA_IDR_IDR5
//#define KEY_SW3_DDR PA_DDR_DDR5
//#define KEY_SW3_CR1 PA_CR1_C15
//#define KEY_SW3_CR2 PA_CR2_C25
//
//#define KEY_SW4 PA_IDR_IDR2
//#define KEY_SW4_DDR PA_DDR_DDR2
//#define KEY_SW4_CR1 PA_CR1_C12
//#define KEY_SW4_CR2 PA_CR2_C22

// #define Receiver_Login PC_IDR_IDR6       // Input   受信机登录键   低电平有效
// #define Receiver_Login_direc PC_DDR_DDR6 // Input   受信机登录键   低电平有效
// #define Receiver_Login_CR1 PC_CR1_C16    // Input   受信机登录键   低电平有效

#define Receiver_Login PA_IDR_IDR2       // Input   受信机登录键   低电平有效
#define Receiver_Login_direc PA_DDR_DDR2 // Input   受信机登录键   低电平有效
#define Receiver_Login_CR1 PA_CR1_C12    // Input   受信机登录键   低电平有效
#define Receiver_Login_CR2 PA_CR2_C22    // Input   受信机登录键   低电平有效

#define KEY_Empty 0
#define KEY_SW2_Down 1
#define KEY_SW3_Down 2
#define KEY_SW4_Down 3

/**受信机使用的IO HA 异常信号 寄存器 */
#define HA_L_signal PA_IDR_IDR4       // Input   HA 下限信号   低电平有效
#define HA_L_signal_direc PA_DDR_DDR4 // Input   HA 下限信号   低电平有效
#define HA_L_signal_CR1 PA_CR1_C14    // Input   HA 下限信号   低电平有效
#define HA_L_signal_CR2 PA_CR2_C24

#define HA_ERR_signal PA_IDR_IDR5       // Input   HA 异常信号  低电平有效
#define HA_ERR_signal_direc PA_DDR_DDR5 // Input   HA 异常信号  低电平有效
#define HA_ERR_signal_CR1 PA_CR1_C15    // Input   HA 异常信号  低电平有效
#define HA_ERR_signal_CR2 PA_CR2_C25

#define HA_Sensor_signal PA_IDR_IDR3       // Input   HA 传感器信号  低电平有效
#define HA_Sensor_signal_direc PA_DDR_DDR3 // Input   HA 传感器信号  低电平有效
#define HA_Sensor_signal_CR1 PA_CR1_C13    // Input   HA 传感器信号  低电平有效
#define HA_Sensor_signal_CR2 PA_CR2_C23

/**WORK/TEST切换引脚寄存器*****/
#define WORK_TEST PB_IDR_IDR0     // Input 受信机测试脚  高电平有效
#define WORK_TEST_DDR PB_DDR_DDR0 // Input 受信机测试脚  高电平有效
#define WORK_TEST_CR1 PB_CR1_C10  // Input 受信机测试脚  高电平有效
#define WORK_TEST_CR2 PB_CR2_C20  // Input 受信机测试脚  高电平有效

#define Receiver_test PB_IDR_IDR0       // Input 受信机测试脚  高电平有效
#define Receiver_test_direc PB_DDR_DDR0 // Input 受信机测试脚  高电平有效
#define Receiver_test_CR1 PB_CR1_C10    // Input 受信机测试脚  高电平有效

/********************天线切换 CG2214M6控制引脚寄存器*****************************************/
#define CG2214M6_VC1 PB_ODR_ODR2     //VC1
#define CG2214M6_VC1_DDR PB_DDR_DDR2 //方向
#define CG2214M6_VC1_CR1 PB_CR1_C12  //模式MODE
#define CG2214M6_VC1_CR2 PB_CR2_C22  //输出频率或者中断控制

#define CG2214M6_VC2 PB_ODR_ODR3     //VC2
#define CG2214M6_VC2_DDR PB_DDR_DDR3 //方向
#define CG2214M6_VC2_CR1 PB_CR1_C13  //模式MODE
#define CG2214M6_VC2_CR2 PB_CR2_C23  //输出频率或者中断控制

#define CG2214M6_VC1_USE  \
    {                     \
        CG2214M6_VC1 = 1; \
        CG2214M6_VC2 = 0; \
    }
#define CG2214M6_VC2_USE  \
    {                     \
        CG2214M6_VC2 = 0; \
        CG2214M6_VC2 = 1; \
    }

#define CG2214M6_USE_T CG2214M6_VC2_USE
#define CG2214M6_USE_R CG2214M6_VC1_USE

/******************以下是data寄存器************************************/

// #define Receiver_vent PC_IDR_IDR5       // Input   受信机换气联动ON/OFF
// #define Receiver_vent_direc PC_DDR_DDR5 // Input   受信机换气联动ON/OFF
// #define Receiver_vent_CR1 PC_CR1_C15    // Input   受信机换气联动ON/OFF
#define Receiver_vent Receiver_vent_Cache       // Input   受信机换气联动ON/OFF
#define Receiver_vent_direc Receiver_vent_Cache // Input   受信机换气联动ON/OFF
#define Receiver_vent_CR1 Receiver_vent_Cache   // Input   受信机换气联动ON/OFF

#define PIN_BEEP PA_ODR_ODR0       // Output   蜂鸣器
#define PIN_BEEP_direc PA_DDR_DDR0 // Output   蜂鸣器
#define PIN_BEEP_CR1 PA_CR1_C10    // Output   蜂鸣器

#define Receiver_OUT_OPEN PD_ODR_ODR3       // Output   受信机继电器OPEN  高电平有效
#define Receiver_OUT_OPEN_direc PD_DDR_DDR3 // Output   受信机继电器OPEN  高电平有效
#define Receiver_OUT_OPEN_CR1 PD_CR1_C13    // Output   受信机继电器OPEN  高电平有效

#define Receiver_OUT_CLOSE PD_ODR_ODR2       // Output   受信机继电器close  高电平有效
#define Receiver_OUT_CLOSE_direc PD_DDR_DDR2 // Output   受信机继电器close  高电平有效
#define Receiver_OUT_CLOSE_CR1 PD_CR1_C12    // Output   受信机继电器close  高电平有效

#define Receiver_OUT_STOP PD_ODR_ODR1       // Output   受信机继电器stop  高电平有效
#define Receiver_OUT_STOP_direc PD_DDR_DDR1 // Output   受信机继电器stop  高电平有效
#define Receiver_OUT_STOP_CR1 PD_CR1_C11    // Output   受信机继电器stop  高电平有效

#define Receiver_OUT_VENT PD_ODR_ODR0       // Output   受信机继电器VENT  高电平有效
#define Receiver_OUT_VENT_direc PD_DDR_DDR0 // Output 受信机继电器VENT  高电平有效
#define Receiver_OUT_VENT_CR1 PD_CR1_C10    // Output 受信机继电器VENT  高电平有效

#define Inverters_OUT PA_IDR_IDR2       // 输入   继电器输出信号反向   低电平有效
#define Inverters_OUT_direc PA_DDR_DDR2 // 输入   继电器输出信号反向   低电平有效
#define Inverters_OUT_CR1 PA_CR1_C12    // 输入   继电器输出信号反向   低电平有效

/*********************************************************************************/

#endif
