#ifndef __RAM_H__
#define __RAM_H__
//CPU?????
//?????ADF030???????
//#define TX_TEST_CARRIER_MODE 0
#include "initial.h" // 锟斤拷始锟斤拷  预锟斤拷锟斤拷

extern uFLAG FLAG_test1;
//************************************************
#define FLAG_test1_BYTE FLAG_test1.BYTE
//------------------------------------------------
#define BIT_SIO FLAG_test1.BIT.Bit0
#define FG_auto_out FLAG_test1.BIT.Bit1
#define FG_OUT_OPEN_CLOSE FLAG_test1.BIT.Bit2 //锟斤拷史锟斤拷录   锟斤拷锟斤拷锟绞诧拷锟斤拷锟斤拷 \
											  //    #define		FG_auto_outbz		FLAG_test1.BIT.Bit3
#define FG_auto_open_time FLAG_test1.BIT.Bit4
#define FG_auto_manual_mode FLAG_test1.BIT.Bit5 //1=auto,0=manual
#define FG_Receiver_LED_RX FLAG_test1.BIT.Bit6
#define FG_First_auto FLAG_test1.BIT.Bit7
//************************************************
//************************************************
extern volatile uFLAG FLAG_test;
#define FLAG_test_BYTE FLAG_test.BYTE
//------------------------------------------------
#define FLAG_Receiver_Scanning FLAG_test.BIT.Bit0
#define FG_test_tx_1010 FLAG_test.BIT.Bit1
#define X_HIS FLAG_test.BIT.Bit2 //鍘嗗彶璁板綍   璇?鐮佺巼娴嬭瘯鐢?
#define FG_test_tx_on FLAG_test.BIT.Bit3
#define FG_test_tx_off FLAG_test.BIT.Bit4
#define FG_test_mode FLAG_test.BIT.Bit5
#define FG_test1 FLAG_test.BIT.Bit6
#define FG_test_rx FLAG_test.BIT.Bit7
//************************************************
//************************************************
extern volatile uFLAG FLAG0;
#define FLAG0_BYTE FLAG0.BYTE
//------------------------------------------------
#define FLAG_Receiver_IDCheck FLAG0.BIT.Bit0
#define FLAG_Signal_DATA_OK FLAG0.BIT.Bit1
#define FLAG_APP_RX FLAG0.BIT.Bit2
#define FLAG_IDCheck_OK FLAG0.BIT.Bit3
#define FLAG_ID_Erase_Login FLAG0.BIT.Bit4
#define FLAG_ID_Erase_Login_PCS FLAG0.BIT.Bit5
#define FLAG_ID_Login FLAG0.BIT.Bit6
#define FLAG_ID_Login_OK FLAG0.BIT.Bit7
//************************************************
extern volatile uFLAG FLAG1;
#define FLAG1_BYTE FLAG1.BYTE
//------------------------------------------------
#define FLAG_Receiver_BEEP FLAG1.BIT.Bit0
#define FLAG_ID_Login_EXIT FLAG1.BIT.Bit1
#define FLAG_ID_Login_OK_bank FLAG1.BIT.Bit2
#define FG_beep_on FLAG1.BIT.Bit3
#define FG_beep_off FLAG1.BIT.Bit4
#define FG_allow_out FLAG1.BIT.Bit5
#define FG_NOT_allow_out FLAG1.BIT.Bit6
#define FG_10ms FLAG1.BIT.Bit7
//************************************************

extern u16 X_COUNT;
extern u16 X_ERR; //记录错误的个数
extern u16 X_ERRTimer;

extern u8 SIO_cnt;
extern u8 SIO_buff[16];
extern u8 SIO_DATA[16];
extern u8 Tx_Rx_mode;

extern u8 TIME_10ms;
extern u16 TIMER1s;
extern u16 TIMER300ms;
extern u16 TIMER18ms;
extern u8 TIMER250ms_STOP;
extern u16 TIME_auto_out;
extern u16 TIME_auto_close;
extern u16 time_3sec;
extern u32 ID_Receiver_DATA[256]; //写入EEPROM ID缓存
extern u16 ID_DATA_PCS;
extern u32 DATA_Packet_ID;
extern u8 DATA_Packet_Control;
extern u8 DATA_Packet_Contro_buf; //2015.3.24淇?姝?
extern u32 ID_Receiver_Login;
extern u8 TIME_EMC;

extern u16 INquiry;
extern u16 TIME_Receiver_Login_restrict;
extern u8 COUNT_Receiver_Login;
extern u16 TIME_Receiver_Login;
extern u16 TIME_Login_EXIT_rest;
extern u16 TIME_Receiver_Login_led;

extern u8 TIME_OUT_OPEN_CLOSE;
extern u16 TIME_Receiver_LED_OUT;
extern u16 TIME_Login_EXIT_Button;
extern u16 Manual_override_TIMER;
extern u16 time_Login_exit_256;
extern u16 TIME_Fine_Calibration; //绐勫甫涓嬩腑棰戞护娉㈠櫒100KHz绮炬牎

extern u8 Count_key_SW3;
extern u8 Display_key_SW3;
extern ADF70XX_REG_T ROM_adf7030_value[16];
extern const ADF70XX_REG_T Default_adf7030_value[16];
/**********LCD******************/
extern uFLAG FlagLCDUpdate;

#define LCDUpdateIDFlag FlagLCDUpdate.BIT.Bit0
#define LCDUpdateRISSFlag FlagLCDUpdate.BIT.Bit1
#define LCDFirstDisplayFlag FlagLCDUpdate.BIT.Bit2
/**********RSSI*********************/
extern short RAM_RSSI_AVG;
extern long RAM_RSSI_SUM;
extern u8 RSSI_Read_Counter;
extern u8 Flag_RSSI_Read_Timer;

#define SPI_SEND_BUFF_LONG 300
#define SPI_REV_BUFF_LONG 40

extern const u8 ADF7030Cfg[];
u32 CFG_SIZE(void);

#define OPEN_LONG 12
extern u8 CONST_TXPACKET_DATA_20000AF0[OPEN_LONG];

extern u32 GENERIC_PKT_TEST_MODES0_32bit_20000548;
extern const u8 TEST_MODES0_para[5];
extern u32 RADIO_DIG_TX_CFG0_32bit_20000304;
#define AFCCONFIG_LONG 4
extern const u8 CONST_AFC_Configuration_400041F8[4];
extern u16 PA_POWER_OUT[2][18];
extern u32 PROFILE_CH_FREQ_32bit_200002EC;
extern u32 PROFILE_RADIO_AFC_CFG1_32bit_2000031C;
void SPI_INIT(void);
u32 ADF7030_GET_MISC_FW(void); //??MISC_FW?????
void ADF7030_WRITING_PROFILE_FROM_POWERON(void);
void ADF7030_TRANSMITTING_FROM_POWEROFF(void);
void ADF7030_TRANSMITTING_FROM_RX(void);
void ADF7030_RECEIVING_FROM_POWEROFF(void);
void ADF7030_RECEIVING_FROM_POWERON(void);
void SCAN_RECEIVE_PACKET(void);
u32 ConfigurationLen(void);

#endif
