/***********************************************************************/
/*  FILE        :lcd.c                                                 */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8L151G6     Crystal: 16M HSI                       */
/*  DESCRIPTION :                                                      */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/

#include <iostm8l151g4.h> // CPU型号
#include "Pin_define.h"   // 管脚定义
#include "ram.h"          // RAM定义
#include "lcd.h"          // RAM定义
#include "ADF7030_1.h"

/******************************************************************************
 * FunctionName: lcd_init
 *
 * Function Description: This function initializes display controller.
 *
 * Function Parameter: None
 *
 * Function Returns: void
 *
*********************************************************************************/
void lcd_init(void)
{
    PIN_LCD_SEL = 0; //Selected LCD
    PIN_LCD_RST = 0; //Reset
    delay(100);
    PIN_LCD_RST = 1; //Reset OK
    delay(20);
    send_command(0xe2); //LCD soft Reset
    delay(5);
    send_command(0x2c); //Booster ratio set1
    delay(5);
    send_command(0x2e); //Booster ratio set2
    delay(5);
    send_command(0x2f); //Booster ratio set3
    delay(5);
    send_command(0x24); //Coarse contrast
    send_command(0x81); //Contrast trimming
    send_command(0x1a); //Contrast trimming ,Value
    send_command(0xa2); //1/9 LCD bias set)
    send_command(0xc8); //Common output mode select
    send_command(0xa0); //Column scanning*/
    send_command(0x40); //The initial line of display settings
    send_command(0xaf); //display ON
    Display_key_SW3 = 0xff;
    FlagLCDUpdate.BYTE = 0x00;
    LCDFirstDisplayFlag = 1;
    lcd_clear(1);
    //    lcd_desplay();
    PIN_LCD_SEL = 1; // not Selected LCD
}

/******************************************************************************
 * FunctionName: send_command
 *
 * Function Description: This function is send command.
 *
 * Function Parameter:
 * data   :   command.
 *
 * Function Returns: void
 *
*********************************************************************************/
void send_command(UINT8 data)
{
    UINT8 i;
    PIN_LCD_SEL = 0;   //Selected LCD
    PIN_LCD_A0_RS = 0; //command
    for (i = 0; i < 8; i++)
    {
        PIN_LCD_SCLK = 0;
        if (data & 0x80)
            PIN_LCD_SDATA = 1;
        else
            PIN_LCD_SDATA = 0;
        PIN_LCD_SCLK = 1;
        data = data << 1;
    }
}
/******************************************************************************
 * FunctionName: send_data
 *
 * Function Description: This function is display data.
 *
 * Function Parameter:
 * data   :   display data.
 *
 * Function Returns: void
 *
*********************************************************************************/
void send_data(UINT8 data)
{
    UINT8 i;
    PIN_LCD_SEL = 0; //Selected LCD
    PIN_LCD_A0_RS = 1;
    ; //data
    for (i = 0; i < 8; i++)
    {
        PIN_LCD_SCLK = 0;
        if (data & 0x80)
            PIN_LCD_SDATA = 1;
        else
            PIN_LCD_SDATA = 0;
        PIN_LCD_SCLK = 1;
        data = data << 1;
    }
}

/******************************************************************************
 * FunctionName: delay
 *
 * Function Description: This function is delay.
 *
 * Function Parameter:
 * i   :  delay NUM.
 *
 * Function Returns: void
 *
*********************************************************************************/
void delay(UINT16 i)
{
    UINT16 j, k;
    for (j = 0; j < i; j++)
        for (k = 0; k < 110; k++)
            ;
}

/*************************************************/
//LCD显示(L)*(H 8的倍数)，串行模式
//说明：x,y代表坐标；L和H代表图像大小
/*************************************************/
void display_map_xy(unsigned char x, unsigned char y, unsigned char l, unsigned char h, const unsigned char *p)
{
    unsigned char i;
    unsigned char seg;
    unsigned char page;
    unsigned char com0, com1;
    com1 = x / 16 + 0x10;
    com0 = x % 16;
    for (i = 0; i < h / 8; i++)
    {
        page = 0xb0 + i + y / 8;
        send_command(page);
        send_command(com1); //列地址，高低字节两次写入，从第0 列开始
        send_command(com0);
        for (seg = x; seg < (l + x); seg++) //写128 列
        {
            send_data(*p++);
        }
    }
}
/**
 ****************************************************************************
 * @Function : void display_map_58_6(u8 x,u8 y,u8 l,u8 h,const u8 *p)
 * @File     : lcd.c
 * @Program  :
 * @Created  : 2017/5/4 by Xiaowine
 * @Brief    :
 * @Version  : V1.0
**/
void display_map_58_6(unsigned char x, unsigned char y, unsigned char len, const unsigned char *p)
{
    unsigned char i;
    for (i = 0; i < len; i++)
        display_map_xy(x + i * 6, y, 5, 8, char_Small + (*p++ - ' ') * 5);
}

void lcd_clear(unsigned char data1)
{
    //------ò???ê????á3ìDò
    unsigned char seg;
    unsigned char page;
    for (page = 0xb0; page < 0xb9; page++) //D′ò3μ??·128 ò3 0xb0----0xb8
    {
        send_command(page);
        send_command(0x10); //áDμ??·￡???μí×??úá?′?D′è?￡?′óμú0 áD?aê?
        send_command(0x00);
        for (seg = 0; seg < 128; seg++) //D′128 áD
        {
            if (data1 == 1)
                send_data(0x00);
            else
                send_data(0xFF);
        }
    }
}

/************************************************************/
void lcd_desplay(void)
{
    UINT8 i, data;
    UINT32 num;

    if (LCDUpdateIDFlag == 1)
    {
        LCDUpdateIDFlag = 0;
        if (LCDFirstDisplayFlag == 1)
        {
            LCDFirstDisplayFlag = 0;
            lcd_clear(1);
        }
        num = DATA_Packet_ID;
        for (i = 0; i < 8; i++)
        {
            data = num % 10;
            num = num / 10;
            display_map_xy(1 + (7 - i) * 9, 24, 7, 16, char_Medium + data * 14);
        }
        num = Packet_counte;

        for (i = 0; i < 3; i++)
        {
            data = num % 10;
            num = num / 10;
            display_map_xy(1 + (7 - i) * 9, 0, 7, 16, char_Medium + data * 14);
        }

        switch (DATA_Packet_Contro_buf)
        {
        case 0x14: //stop+login
            break;
        case 0x40: //自动送信
            break;
        case 0x01: //VENT
            break;
        case 0x02: //close
                   //                    display_map_xy(1,48,8,16,char_Contro+8*16);
                   //                    display_map_xy(1+1*9,48,8,16,char_Contro+9*16);
                   //                    display_map_xy(1+2*9,48,8,16,char_Contro+10*16);
                   //                    display_map_xy(1+3*9,48,8,16,char_Contro+11*16);
                   //                    display_map_xy(1+4*9,48,8,16,char_Contro+12*16);

            display_map_xy(0, 40, 2, 24, char_Contro + 13 * 48);
            display_map_xy(2, 40, 16, 24, char_Contro + 8 * 48);
            display_map_xy(2 + 1 * 16, 40, 16, 24, char_Contro + 9 * 48);
            display_map_xy(2 + 2 * 15, 40, 16, 24, char_Contro + 10 * 48);
            display_map_xy(2 + 3 * 16, 40, 16, 24, char_Contro + 11 * 48);
            display_map_xy(2 + 4 * 16, 40, 16, 24, char_Contro + 12 * 48);
            break;
        case 0x04: //stop
                   //                                display_map_xy(1,48,8,16,char_Contro+4*16);
                   //                                display_map_xy(1+1*9,48,8,16,char_Contro+5*16);
                   //                                display_map_xy(1+2*9,48,8,16,char_Contro+6*16);
                   //                                display_map_xy(1+3*9,48,8,16,char_Contro+7*16);
                   //                                display_map_xy(1+4*9,48,8,16,char_Contro+13*16);  //空

            display_map_xy(0, 40, 2, 24, char_Contro + 13 * 48);
            display_map_xy(2, 40, 16, 24, char_Contro + 4 * 48);
            display_map_xy(2 + 1 * 16, 40, 16, 24, char_Contro + 5 * 48);
            display_map_xy(2 + 2 * 15, 40, 16, 24, char_Contro + 6 * 48);
            display_map_xy(2 + 3 * 16, 40, 16, 24, char_Contro + 7 * 48);
            display_map_xy(2 + 4 * 16, 40, 16, 24, char_Contro + 13 * 48);
            break;
        case 0x08: //open
                   //                                display_map_xy(1,48,8,16,char_Contro+0*16);
                   //                                display_map_xy(1+1*9,48,8,16,char_Contro+1*16);
                   //                                display_map_xy(1+2*9,48,8,16,char_Contro+2*16);
                   //                                display_map_xy(1+3*9,48,8,16,char_Contro+3*16);
                   //                                display_map_xy(1+4*9,48,8,16,char_Contro+13*16);   //空

            display_map_xy(0, 40, 16, 24, char_Contro + 0 * 48);
            display_map_xy(2 + 1 * 16, 40, 16, 24, char_Contro + 1 * 48);
            display_map_xy(2 + 2 * 16, 40, 16, 24, char_Contro + 2 * 48);
            display_map_xy(2 + 3 * 16, 40, 16, 24, char_Contro + 3 * 48);
            display_map_xy(2 + 4 * 16, 40, 16, 24, char_Contro + 13 * 48);
            break;
        case 0x0C: //open+stop
            break;
        case 0x06: //close+stop
            break;
        case 0x0A: //close+OPEN
            break;
        case 0x09: //vent+OPEN
            break;
        case 0x03: //vent+close
            break;
        default:
            break;
        }
    }
}

const unsigned char char_ID[] = { //ID CHECKER
    /*--  宽度x高度=110x24  --*/
    0x00, 0x00, 0x20, 0x20, 0x20, 0xE0, 0x20, 0x20, 0x20, 0x00, 0x00, 0x20, 0xE0, 0x20, 0x20, 0x20,
    0x20, 0x60, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x20, 0x60, 0xE0, 0x00, 0x20, 0xE0, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x20, 0xE0, 0x20, 0x20, 0xE0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x20, 0x60, 0xE0, 0x00, 0x20, 0xE0, 0x20,
    0x00, 0x00, 0x00, 0xA0, 0x60, 0x20, 0x20, 0x00, 0x20, 0xE0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x60, 0x80, 0x00, 0x20, 0xE0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x40, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0xFF, 0x00, 0x00, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x30, 0x08, 0x1C,
    0x23, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0x10, 0x10, 0x10, 0x30, 0xD0, 0x08, 0x08, 0x07, 0x00, 0x00, 0x00, 0x10, 0x10,
    0x10, 0x1F, 0x10, 0x10, 0x10, 0x00, 0x00, 0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x08, 0x0C, 0x06,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06,
    0x08, 0x10, 0x10, 0x10, 0x10, 0x0C, 0x03, 0x00, 0x10, 0x1F, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x1F, 0x10, 0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x18, 0x06, 0x00, 0x00, 0x01,
    0x06, 0x08, 0x10, 0x10, 0x10, 0x10, 0x0C, 0x03, 0x00, 0x10, 0x1F, 0x10, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x14, 0x18, 0x10, 0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x18, 0x06, 0x00, 0x10,
    0x1F, 0x10, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x10, 0x10};

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//
//以下是字库
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//
const unsigned char char_Small[] = {
    /*--  宽度x高度=5x8  --*/
    // ASCII码(HEX)  字符
    0x00, 0x00, 0x00, 0x00, 0x00, //20      空白
    0x00, 0x00, 0xBE, 0x00, 0x00, //21       ！
    0x00, 0x00, 0x00, 0x00, 0x00, //22       “       空
    0x00, 0x00, 0x00, 0x00, 0x00, //23       #        空
    0x2A, 0x2C, 0xF8, 0x2C, 0x2A, //24      ￥
    0x00, 0x00, 0x00, 0x00, 0x00, //25       %        空
    0x00, 0x00, 0x00, 0x00, 0x00, //26       &        空
    0x00, 0xB0, 0x70, 0x00, 0x00, //27       ，
    0x00, 0x38, 0x44, 0x82, 0x00, //28       (
    0x00, 0x82, 0x44, 0x38, 0x00, //29       )
    0x28, 0x10, 0x7C, 0x10, 0x28, //2A       *
    0x10, 0x10, 0x7C, 0x10, 0x10, //2B       +
    0x00, 0x0C, 0x0A, 0x00, 0x00, //2C       '
    0x10, 0x10, 0x10, 0x10, 0x10, //2D       -
    0x00, 0xC0, 0xC0, 0x00, 0x00, //2E       .
    0x40, 0x20, 0x10, 0x08, 0x04, //2F       /

    0x7C, 0xA2, 0x92, 0x8A, 0x7C, //30        0
    0x00, 0x84, 0xFE, 0x80, 0x00, //31        1
    0x84, 0xC2, 0xA2, 0x92, 0x8C, //32        2
    0x42, 0x82, 0x8A, 0x96, 0x62, //33        3
    0x30, 0x28, 0x24, 0xFE, 0x20, //34        4
    0x4E, 0x8A, 0x8A, 0x8A, 0x72, //35        5
    0x78, 0x94, 0x92, 0x92, 0x60, //36        6
    0x06, 0x02, 0xE2, 0x12, 0x0E, //37        7
    0x6C, 0x92, 0x92, 0x92, 0x6C, //38        8
    0x0C, 0x92, 0x92, 0x52, 0x3C, //39        9

    0x00, 0x6C, 0x6C, 0x00, 0x00, //3A        :
    0x00, 0xAC, 0x6C, 0x00, 0x00, //3B        ;
    0x10, 0x28, 0x44, 0x82, 0x00, //3C        <
    0x28, 0x28, 0x28, 0x28, 0x28, //3D        =
    0x00, 0x82, 0x44, 0x28, 0x10, //3E        >
    0x00, 0x00, 0x00, 0x00, 0x00, //3F        ?        空
    0x64, 0x92, 0xF2, 0x82, 0x7C, //40        @

    0xF8, 0x24, 0x22, 0x24, 0xF8, //41        A
    0xFE, 0x92, 0x92, 0x92, 0x6C, //42        B
    0x7C, 0x82, 0x82, 0x82, 0x44, //43        C
    0xFE, 0x82, 0x82, 0x44, 0x38, //44        D
    0xFE, 0x92, 0x92, 0x92, 0x82, //45        E
    0xFE, 0x12, 0x12, 0x12, 0x02, //46        F
    0x7C, 0x82, 0x92, 0x92, 0xF4, //47        G
    0xFE, 0x10, 0x10, 0x10, 0xFE, //48        H
    0x00, 0x82, 0xFE, 0x82, 0x00, //49        I
    0x40, 0x80, 0x82, 0x7E, 0x02, //4A        J
    0xFE, 0x10, 0x28, 0x44, 0x82, //4B        K
    0xFE, 0x80, 0x80, 0x80, 0x80, //4C        L
    0xFE, 0x04, 0x18, 0x04, 0xFE, //4D        M
    0xFE, 0x08, 0x10, 0x20, 0xFE, //4E        N
    0x7C, 0x82, 0x82, 0x82, 0x7C, //4F        O
    0xFE, 0x12, 0x12, 0x12, 0x0C, //50        P
    0x7C, 0x82, 0xA2, 0x42, 0xBC, //51        Q
    0xFE, 0x12, 0x32, 0x52, 0x8C, //52        R
    0x4C, 0x92, 0x92, 0x92, 0x64, //53        S
    0x02, 0x02, 0xFE, 0x02, 0x02, //54        T
    0x7E, 0x80, 0x80, 0x80, 0x7E, //55        U
    0x3E, 0x40, 0x80, 0x40, 0x3E, //56        V
    0x7E, 0x80, 0x70, 0x80, 0x7E, //57        W
    0xC6, 0x28, 0x10, 0x28, 0xC6, //58        X
    0x0E, 0x10, 0xE0, 0x10, 0x0E, //59        Y
    0xC2, 0xA2, 0x92, 0x8A, 0x86, //5A        Z

    0x00, 0xFE, 0x82, 0x82, 0x00, //5B        [
    0x04, 0x08, 0x10, 0x20, 0x40, //5C      符号反斜杠
    0x00, 0x82, 0x82, 0xFE, 0x00, //5D        ]
    0x08, 0x04, 0x02, 0x04, 0x08, //5E        ^
    0x80, 0x80, 0x80, 0x80, 0x80, //5F        _
    0x00, 0x00, 0x00, 0x00, 0x00, //60        '        空

    0x40, 0xA8, 0xA8, 0xA8, 0xF0, //61       a
    0xFE, 0x90, 0x88, 0x88, 0x70, //62       b
    0x70, 0x88, 0x88, 0x88, 0x00, //63       c
    0x70, 0x88, 0x88, 0x90, 0xFE, //64       d
    0x70, 0xA8, 0xA8, 0xA8, 0x30, //65       e
    0x10, 0xFC, 0x12, 0x02, 0x04, //66       f
    0x10, 0xA8, 0xA8, 0xA8, 0x78, //67       g
    0xFE, 0x10, 0x08, 0x08, 0xF0, //68       h
    0x00, 0x90, 0xFA, 0x80, 0x00, //69       i
    0x40, 0x80, 0x88, 0x7A, 0x00, //6A       j
    0xFE, 0x20, 0x50, 0x88, 0x00, //6B       k
    0x00, 0x82, 0xFE, 0x80, 0x00, //6C       l
    0xF8, 0x08, 0x30, 0x08, 0xF8, //6D       m
    0xF8, 0x10, 0x08, 0x08, 0xF0, //6E       n
    0x70, 0x88, 0x88, 0x88, 0x70, //6F       o
    0xF8, 0x28, 0x28, 0x28, 0x10, //70       p
    0x10, 0x28, 0x28, 0x28, 0xF8, //71       q
    0xF8, 0x10, 0x08, 0x08, 0x10, //72       r
    0x90, 0xA8, 0xA8, 0xA8, 0x40, //73       s
    0x08, 0x7E, 0x88, 0x80, 0x40, //74       t
    0x78, 0x80, 0x80, 0x40, 0xF8, //75       u
    0x38, 0x40, 0x80, 0x40, 0x38, //76       v
    0x78, 0x80, 0x60, 0x80, 0x78, //77       w
    0x88, 0x50, 0x20, 0x50, 0x88, //78       x
    0x18, 0xA0, 0xA0, 0xA0, 0x78, //79       y
    0x88, 0xC8, 0xA8, 0x98, 0x88, //7A       z
    0xFE, 0x7C, 0x38, 0x10, 0x00, //7B       三国

    0xF8, 0x24, 0x22, 0x24, 0xF8, //7c        A
    0xFE, 0x92, 0x92, 0x92, 0x6C, //7d        B
    0x7C, 0x82, 0x82, 0x82, 0x44, //7e        C
    0xFE, 0x82, 0x82, 0x44, 0x38, //7f        D
    0xFE, 0x92, 0x92, 0x92, 0x82, //80        E
    0xFE, 0x12, 0x12, 0x12, 0x02, //81        F
    0x28, 0x10, 0x7C, 0x10, 0x28  //82        *
};

const unsigned char char_Medium[] = {
    /*--  宽度x高度=7x16  --*/
    // ASCII码(HEX)  字符
    0xF8, 0xFC, 0x06, 0x86, 0xC6, 0xFC, 0xF8, 0x3F, 0x7F, 0xC7, 0xC3, 0xC1, 0x7F, 0x3F, //30         0
    0x00, 0x18, 0x1C, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xFF, 0xFF, 0xC0, 0xC0, //31         1
    0x1C, 0x1E, 0x0E, 0x06, 0x86, 0xFE, 0xFC, 0xF8, 0xFC, 0xCE, 0xC7, 0xC3, 0xC1, 0xC0, //32         2
    0x38, 0x3C, 0x86, 0x86, 0x86, 0xFC, 0x78, 0x38, 0x78, 0xC1, 0xC1, 0xC1, 0x7F, 0x3E, //33         3
    0x80, 0xE0, 0xF8, 0x1C, 0xFE, 0xFE, 0x00, 0x1F, 0x1F, 0x18, 0x18, 0xFF, 0xFF, 0x18, //34         4
    0xFE, 0xFE, 0xC6, 0xC6, 0xC6, 0x86, 0x86, 0x31, 0x71, 0xC0, 0xC0, 0xC0, 0x7F, 0x3F, //35         5
    0xF8, 0xFC, 0xCE, 0xC6, 0xC6, 0x86, 0x86, 0x3F, 0x7F, 0xC0, 0xC0, 0xC0, 0x7F, 0x3F, //36         6
    0x1E, 0x1E, 0x06, 0x06, 0x86, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x01, 0x00, //37         7
    0xF8, 0xFC, 0x86, 0x86, 0x86, 0xFC, 0xF8, 0x3E, 0x7F, 0xC1, 0xC1, 0xC1, 0x7F, 0x3E, //38         8
    0xFC, 0xFE, 0x06, 0x06, 0x06, 0xFE, 0xFC, 0x01, 0xC3, 0xC6, 0xC6, 0xE6, 0x7F, 0x3F, //39         9
    0xF0, 0xFC, 0x0E, 0x06, 0x0E, 0xFC, 0xF0, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0xFF, 0xFF, //3A         A
    0xFE, 0xFE, 0x06, 0x06, 0x8E, 0xFC, 0xF8, 0xFF, 0xFF, 0xC3, 0xC3, 0xC7, 0x7F, 0x3C, //3B         B
    0xF8, 0xFC, 0x06, 0x06, 0x06, 0x1E, 0x1C, 0x3F, 0x7F, 0xC0, 0xC0, 0xC0, 0xF0, 0x70, //3C         C
    0xFE, 0xFE, 0x06, 0x06, 0x0E, 0xFC, 0xF8, 0xFF, 0xFF, 0xC0, 0xC0, 0xE0, 0x7F, 0x3F, //3D         D
    0xFE, 0xFE, 0x86, 0x86, 0x86, 0x86, 0x06, 0xFF, 0xFF, 0xC1, 0xC1, 0xC1, 0xC1, 0xC0, //3E         E
    0xFE, 0xFE, 0x86, 0x86, 0x86, 0x86, 0x06, 0xFF, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x00, //3F         F
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0x00, 0x00, //40         .
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, //41         -
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //42          清屏
    0x0C, 0x18, 0x70, 0xFE, 0xFE, 0x70, 0x1C, 0xC3, 0x63, 0x1B, 0xFF, 0xFF, 0x3B, 0x63  //43        *
};

const unsigned char char_Contro[] = {
    ///*--  文字:  O  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,
    //
    ///*--  文字:  P  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,
    //
    ///*--  文字:  E  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,
    //
    ///*--  文字:  N  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,
    //
    ///*--  文字:  S  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,
    //
    ///*--  文字:  T  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,
    //
    ///*--  文字:  O  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,
    //
    ///*--  文字:  P  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,
    //
    ///*--  文字:  C  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,
    //
    ///*--  文字:  L  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,
    //
    ///*--  文字:  O  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,
    //
    ///*--  文字:  S  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,
    //
    ///*--  文字:  E  --*/
    ///*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
    //0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,
    //
    ///*--  文字:  空  --*/
    //0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

    //// O(0) P(1) E(2)
    //// N(3) S(4) T(5)
    //// O(6) P(7) C(8)
    //// L(9) O(10) S(11)
    //// E(12)
    //
    //0xC0,0xE0,0x30,0x10,0x10,0x10,0x30,0xE0,0x03,0x07,0x0C,0x08,0x08,0x08,0x0C,0x07,/*"O",0*/
    //
    //0xF0,0xF0,0x10,0x10,0x10,0xF0,0xE0,0x00,0x0F,0x0F,0x01,0x01,0x01,0x00,0x00,0x00,/*"P",1*/
    //
    //0xF0,0xF0,0x90,0x90,0x90,0x10,0x00,0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00,0x00,/*"E",2*/
    //
    //0xF0,0xF0,0x70,0xC0,0x80,0x00,0xF0,0xF0,0x0F,0x0F,0x00,0x01,0x03,0x06,0x0F,0x0F,/*"N",3*/
    //
    //0x60,0xF0,0x90,0x90,0x90,0x10,0x00,0x00,0x08,0x08,0x08,0x09,0x09,0x0F,0x07,0x00,/*"S",4*/
    //
    //0x10,0x10,0xF0,0xF0,0x10,0x10,0x10,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,/*"T",5*/
    //
    //0xC0,0xE0,0x30,0x10,0x10,0x10,0x30,0xE0,0x03,0x07,0x0C,0x08,0x08,0x08,0x0C,0x07,/*"O",6*/
    //
    //0xF0,0xF0,0x10,0x10,0x10,0xF0,0xE0,0x00,0x0F,0x0F,0x01,0x01,0x01,0x00,0x00,0x00,/*"P",7*/
    //
    //0xC0,0xE0,0x20,0x10,0x10,0x10,0x10,0x00,0x03,0x07,0x0C,0x08,0x08,0x08,0x08,0x00,/*"C",8*/
    //
    //0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00,0x00,/*"L",9*/
    //
    //0xC0,0xE0,0x30,0x10,0x10,0x10,0x30,0xE0,0x03,0x07,0x0C,0x08,0x08,0x08,0x0C,0x07,/*"O",10*/
    //
    //0x60,0xF0,0x90,0x90,0x90,0x10,0x00,0x00,0x08,0x08,0x08,0x09,0x09,0x0F,0x07,0x00,/*"S",11*/
    //
    //0xF0,0xF0,0x90,0x90,0x90,0x10,0x00,0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00,0x00,/*"E",12*/
    ///*--  文字:  空  --*/
    //0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

    // O(0) P(1) E(2)
    // N(3) S(4) T(5)
    // O(6) P(7) C(8)
    // L(9) O(10) S(11)
    // E(12)

    0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00,
    0x7C, 0xFE, 0xFF, 0x83, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x83, 0xFF, 0xFF, 0x7C,
    0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x06, 0x06, 0x06, 0x06, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, /*"O",0*/

    0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x19, 0x1F, 0x1F, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"P",1*/

    0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, /*"E",2*/

    0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xFF, 0x03, 0x0F, 0x1E, 0x3C, 0xF8, 0xE0, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x07, 0x00, 0x00, /*"N",3*/

    0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x0F, 0x0F, 0x1C, 0x18, 0x38, 0x38, 0x38, 0x70, 0xF1, 0xE1, 0xC0, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, /*"S",4*/

    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"T",5*/

    0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00,
    0x7C, 0xFE, 0xFF, 0x83, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x83, 0xFF, 0xFF, 0x7C,
    0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x06, 0x06, 0x06, 0x06, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, /*"O",6*/

    0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x19, 0x1F, 0x1F, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"P",7*/

    0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7C, 0xFE, 0xFF, 0xC3, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, /*"C",8*/

    0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, /*"L",9*/

    0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00,
    0x7C, 0xFE, 0xFF, 0x83, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x83, 0xFF, 0xFF, 0x7C,
    0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x06, 0x06, 0x06, 0x06, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, /*"O",10*/

    0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x0F, 0x0F, 0x1C, 0x18, 0x38, 0x38, 0x38, 0x70, 0xF1, 0xE1, 0xC0, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, /*"S",11*/

    0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, /*"E",12*/

    /*--  文字:  空  --*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char char_RSSI[] = {
    /*--  调入了一幅图像：D:\My Documents\工作资料\日本文化\文化改型\TCXO版 LCD图像数据\NULL.bmp  --*/
    /*--  宽度x高度=16x16  --*/
    0xE0, 0xF8, 0x1C, 0x0E, 0x06, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x06, 0x0E, 0x1C, 0xF8, 0xE0,
    0x07, 0x1F, 0x38, 0x70, 0x60, 0xC0, 0xC0, 0x80, 0x80, 0xC0, 0xC0, 0x60, 0x70, 0x38, 0x1F, 0x07,
    /*--  调入了一幅图像：D:\My Documents\工作资料\日本文化\文化改型\TCXO版 LCD图像数据\o.bmp  --*/
    /*--  宽度x高度=16x16  --*/
    0xE0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xF8, 0xE0,
    0x07, 0x1F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F, 0x07};
