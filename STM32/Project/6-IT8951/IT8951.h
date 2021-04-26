/**
  ******************************************************************************
  * @author  Waveshare Team
  ******************************************************************************
**/

#ifndef _IT8951_H
#define _IT8951_H

#include "stm32f4xx.h"
#include "usart.h"

#define IT8951_Interface_I80
//#define IT8951_Interface_SPI
//#define IT8951_Interface_I2C

#define SET_DC 						GPIO_SetBits(GPIOC, GPIO_Pin_7)
#define CLR_DC						GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#define SET_CS 						GPIO_SetBits(GPIOC, GPIO_Pin_6)
#define CLR_CS						GPIO_ResetBits(GPIOC, GPIO_Pin_6)
#define SET_WEN 					GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define CLR_WEN						GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define SET_RD 						GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define CLR_RD						GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define SET_RESET 				GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define CLR_RESET					GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define WRITE_DATA(data)	GPIO_Write(GPIOB, data)
#define READ_DATA					GPIO_ReadInputData(GPIOB)
#define READ_RDY					GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)

typedef unsigned          char TByte; //uint8_t
typedef unsigned short     int TWord;	//uint16_t
typedef unsigned           int TDWord;//uint32_t

//prototype of structure
//structure prototype 1
typedef struct IT8951LdImgInfo
{
    TWord usEndianType; //little or Big Endian
    TWord usPixelFormat; //bpp
    TWord usRotate; //Rotate mode
    TDWord ulStartFBAddr; //Start address of source Frame buffer
    TDWord ulImgBufBaseAddr;//Base address of target image buffer
    
}IT8951LdImgInfo;

//structure prototype 2
typedef struct IT8951AreaImgInfo
{
    TWord usX;
    TWord usY;
    TWord usWidth;
    TWord usHeight;
    
}IT8951AreaImgInfo;

typedef struct
{
    TWord usPanelW;
    TWord usPanelH;
    TWord usImgBufAddrL;
    TWord usImgBufAddrH;
    TWord usFWVersion[8]; 	//16 Bytes String
    TWord usLUTVersion[8]; 	//16 Bytes String
}I80IT8951DevInfo;

//Built in I80 Command Code
#define IT8951_TCON_SYS_RUN      0x0001
#define IT8951_TCON_STANDBY      0x0002
#define IT8951_TCON_SLEEP        0x0003
#define IT8951_TCON_REG_RD       0x0010
#define IT8951_TCON_REG_WR       0x0011
#define IT8951_TCON_MEM_BST_RD_T 0x0012
#define IT8951_TCON_MEM_BST_RD_S 0x0013
#define IT8951_TCON_MEM_BST_WR   0x0014
#define IT8951_TCON_MEM_BST_END  0x0015
#define IT8951_TCON_LD_IMG       0x0020
#define IT8951_TCON_LD_IMG_AREA  0x0021
#define IT8951_TCON_LD_IMG_END   0x0022

//I80 User defined command code
#define USDEF_I80_CMD_DPY_AREA     0x0034
#define USDEF_I80_CMD_GET_DEV_INFO 0x0302
#define USDEF_I80_CMD_DPY_BUF_AREA 0x0037
//Panel
#define IT8951_PANEL_WIDTH   1024 //it Get Device information
#define IT8951_PANEL_HEIGHT   758

//Rotate mode
#define IT8951_ROTATE_0     0
#define IT8951_ROTATE_90    1
#define IT8951_ROTATE_180   2
#define IT8951_ROTATE_270   3

//Pixel mode , BPP - Bit per Pixel
#define IT8951_2BPP   0
#define IT8951_3BPP   1
#define IT8951_4BPP   2
#define IT8951_8BPP   3

//Waveform Mode
#define IT8951_MODE_0   0
#define IT8951_MODE_1   1
#define IT8951_MODE_2   2
#define IT8951_MODE_3   3
#define IT8951_MODE_4   4

//Endian Type
#define IT8951_LDIMG_L_ENDIAN   0
#define IT8951_LDIMG_B_ENDIAN   1

//Auto LUT
#define IT8951_DIS_AUTO_LUT   0
#define IT8951_EN_AUTO_LUT    1

//LUT Engine Status
#define IT8951_ALL_LUTE_BUSY 0xFFFF

//-----------------------------------------------------------------------
// IT8951 TCon Registers defines
//-----------------------------------------------------------------------
//Register Base Address
#define DISPLAY_REG_BASE 0x1000               //Register RW access for I80 only

//Base Address of Basic LUT Registers
#define LUT0EWHR  (DISPLAY_REG_BASE + 0x00)   //LUT0 Engine Width Height Reg
#define LUT0XYR   (DISPLAY_REG_BASE + 0x40)   //LUT0 XY Reg
#define LUT0BADDR (DISPLAY_REG_BASE + 0x80)   //LUT0 Base Address Reg
#define LUT0MFN   (DISPLAY_REG_BASE + 0xC0)   //LUT0 Mode and Frame number Reg
#define LUT01AF   (DISPLAY_REG_BASE + 0x114)  //LUT0 and LUT1 Active Flag Reg

//Update Parameter Setting Register
#define UP0SR (DISPLAY_REG_BASE + 0x134)      //Update Parameter0 Setting Reg

#define UP1SR     (DISPLAY_REG_BASE + 0x138)  //Update Parameter1 Setting Reg
#define LUT0ABFRV (DISPLAY_REG_BASE + 0x13C)  //LUT0 Alpha blend and Fill rectangle Value
#define UPBBADDR  (DISPLAY_REG_BASE + 0x17C)  //Update Buffer Base Address
#define LUT0IMXY  (DISPLAY_REG_BASE + 0x180)  //LUT0 Image buffer X/Y offset Reg
#define LUTAFSR   (DISPLAY_REG_BASE + 0x224)  //LUT Status Reg (status of All LUT Engines)

#define BGVR      (DISPLAY_REG_BASE + 0x250)  //Bitmap (1bpp) image color table

//-------System Registers----------------
#define SYS_REG_BASE 0x0000

//Address of System Registers
#define I80CPCR (SYS_REG_BASE + 0x04)

//-------Memory Converter Registers----------------
#define MCSR_BASE_ADDR 0x0200
#define MCSR (MCSR_BASE_ADDR  + 0x0000)
#define LISAR (MCSR_BASE_ADDR + 0x0008)

void InterfaceInit(void);
void HostInit(void);
void IT8951DisplayExample(void);
void IT8951DisplayExample2(void);
void IT8951Display1bppExample2(void);
void IT8951DisplayExample3(void);
void IT8951DisplayClear(void);

//temp
// TWord IT8951ReadReg(TWord usRegAddr);
// void IT8951SetImgBufBaseAddr(TDWord ulImgBufAddr);
// void LCDWaitForReady(void);
// void GetIT8951SystemInfo(void* pBuf);
// void gpio_i80_16b_cmd_out(TWord usCmd);
// void GPIO_Configuration_Out(void);
// void GPIO_Configuration_In(void);
// TWord IT8951ReadReg(TWord usRegAddr);
// void IT8951WriteReg(TWord usRegAddr,TWord usValue);

#endif /*_IT8951_H*/
