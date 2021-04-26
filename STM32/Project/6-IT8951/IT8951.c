/**
  ******************************************************************************
  * @author  Waveshare Team
  ******************************************************************************
**/
#include "IT8951.h"
#include <string.h>
#include "delay.h"

#if defined (IT8951_Interface_I80)
void delay(void)
{
	TWord i,j;
	for (i=0;i<10000;i++)
		for (j=0;j<10;j++);
}

void delay_nop(void)
{
	TWord i;
	for (i=0;i<1000;i++);	
}

void InterfaceInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	delay();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	SET_RESET; 
	SET_WEN; 
	SET_RD; 
	SET_CS; 
	SET_DC; 
	WRITE_DATA(0xffff);
	CLR_RESET;
	delay();
	SET_RESET;
	delay();
}

void GPIO_Configuration_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void GPIO_Configuration_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LCDWaitForReady(void)
{
	TByte ulData = READ_RDY;
	while(ulData == 0)
	{
		ulData = READ_RDY;
		
	}
}

void gpio_i80_16b_cmd_out(TWord usCmd)
{
	GPIO_Configuration_Out();

	LCDWaitForReady();

	CLR_DC;
	CLR_CS;
	CLR_WEN;
	WRITE_DATA(usCmd);
	SET_WEN;
	SET_CS;
}

//-------------------------------------------------------------------
//Host controller Write Data for 16 bits using GPIO simulation
//-------------------------------------------------------------------
void gpio_i80_16b_data_out(TWord usData)
{
	GPIO_Configuration_Out();

	LCDWaitForReady();

	SET_DC;
	CLR_CS;
	CLR_WEN;
	WRITE_DATA(usData);  
	SET_WEN; 
	SET_CS; 
}
//-------------------------------------------------------------------
//Host controller Read Data for 16 bits using GPIO simulation
//-------------------------------------------------------------------
TWord gpio_i80_16b_data_in(void)
{
	TWord usData;

	GPIO_Configuration_In();

	LCDWaitForReady();

	SET_DC;
	CLR_CS;
	CLR_RD;
	delay_nop();
	usData = READ_DATA;
	SET_RD;
	SET_CS;

	return usData;
}

//-----------------------------------------------------------------
//Host controller function 2  Write command code to host data Bus
//-----------------------------------------------------------------
void LCDWriteCmdCode(TWord usCmdCode)
{
	//wait for ready
	LCDWaitForReady();
	//write cmd code
	gpio_i80_16b_cmd_out(usCmdCode);
}

//-----------------------------------------------------------
//Host controller function 3  Write Data to host data Bus
//-----------------------------------------------------------
void LCDWriteData(TWord usData)
{
	//wait for ready
	LCDWaitForReady();
	//write data
	gpio_i80_16b_data_out(usData);
}

//-----------------------------------------------------------
//Host controller function 4  Read Data from host data Bus
//-----------------------------------------------------------
TWord LCDReadData(void)
{
	TWord usData;
	//wait for ready
	LCDWaitForReady();
	//read data from host data bus
	usData = gpio_i80_16b_data_in();
	return usData;
}

#endif

#if defined (IT8951_Interface_SPI)
void delay()
{
	TWord i,j;
	for (i=0;i<10000;i++)
		for (j=0;j<10;j++);
}

void delay_nop()
{
	TWord i;
	for (i=0;i<50;i++);	
}

#define Open_RCC_APB2Periph_SPIx   	       	RCC_APB2Periph_SPI4

#define Open_SPIx                           SPI4
#define Open_SPIx_CLK                       RCC_APB2Periph_SPI4
#define Open_SPIx_CLK_INIT                  RCC_APB1PeriphClockCmd
#define Open_SPIx_IRQn                      SPI4_IRQn
#define Open_SPIx_IRQHANDLER                SPI4_IRQHandler

#define Open_SPIx_SCK_PIN                   GPIO_Pin_12
#define Open_SPIx_SCK_GPIO_PORT             GPIOE
#define Open_SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOE
#define Open_SPIx_SCK_SOURCE                GPIO_PinSource12
#define Open_SPIx_SCK_AF                    GPIO_AF_SPI4

#define Open_SPIx_MISO_PIN                  GPIO_Pin_13
#define Open_SPIx_MISO_GPIO_PORT            GPIOE
#define Open_SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define Open_SPIx_MISO_SOURCE               GPIO_PinSource13
#define Open_SPIx_MISO_AF                   GPIO_AF_SPI4

#define Open_SPIx_MOSI_PIN                  GPIO_Pin_14
#define Open_SPIx_MOSI_GPIO_PORT            GPIOE
#define Open_SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define Open_SPIx_MOSI_SOURCE               GPIO_PinSource14
#define Open_SPIx_MOSI_AF                   GPIO_AF_SPI4

#define RCC_GPIO_CS													RCC_AHB1Periph_GPIOE
#define RCC_AHBxPeriphClockCmd							RCC_AHB1PeriphClockCmd
#define GPIO_PIN_CS													GPIO_Pin_11
#define GPIO_CS_PORT												GPIOE

#define SPI_Enable 													GPIO_ResetBits(GPIO_CS_PORT,GPIO_PIN_CS)
#define SPI_Disable 												GPIO_SetBits(GPIO_CS_PORT,GPIO_PIN_CS)

void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStruct;	 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(Open_SPIx_SCK_GPIO_CLK | Open_SPIx_MISO_GPIO_CLK | Open_SPIx_MOSI_GPIO_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(Open_RCC_APB2Periph_SPIx,ENABLE);
	
	GPIO_PinAFConfig(Open_SPIx_SCK_GPIO_PORT, Open_SPIx_SCK_SOURCE,  Open_SPIx_MOSI_AF);
	GPIO_PinAFConfig(Open_SPIx_MISO_GPIO_PORT, Open_SPIx_MISO_SOURCE, Open_SPIx_MOSI_AF);
	GPIO_PinAFConfig(Open_SPIx_MOSI_GPIO_PORT, Open_SPIx_MOSI_SOURCE, Open_SPIx_MOSI_AF);
	
	GPIO_InitStructure.GPIO_Pin = Open_SPIx_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;  
	GPIO_Init(Open_SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Open_SPIx_MISO_PIN;
	GPIO_Init(Open_SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Open_SPIx_MOSI_PIN;
	GPIO_Init(Open_SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

	SPI_I2S_DeInit(Open_SPIx);
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(Open_SPIx, &SPI_InitStruct);

	SPI_Cmd(Open_SPIx, ENABLE);
}

void SPI_Send_byte(u16 data)
{
	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(Open_SPIx,data);

	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(Open_SPIx);
}

u16 SPI_Receive_byte(void)
{
	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(Open_SPIx,0x00);
	
	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(Open_SPIx);
}

void InterfaceInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	SPI_Configuration();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  	
  SPI_Disable;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//RST PC5
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	delay();
	GPIO_SetBits(GPIOC,GPIO_Pin_5);
	delay();
}

void LCDWaitForReady()
{
	TByte ulData = READ_RDY;
	while(ulData == 0)
	{
		ulData = READ_RDY;
	}
}

//-----------------------------------------------------------------
//Host controller function 2   Write command code to host data Bus
//-----------------------------------------------------------------
void LCDWriteCmdCode(TWord usCmdCode)
{
	//Set Preamble for Write Command
	TWord wPreamble = 0x6000; 

	SPI_Enable;
	
	LCDWaitForReady();
	SPI_Send_byte(wPreamble);		
	
	LCDWaitForReady();
	SPI_Send_byte(usCmdCode);			
	
	delay_nop();
	
	SPI_Disable;
}

//-----------------------------------------------------------
//Host controller function 3    Write Data to host data Bus
//-----------------------------------------------------------
void LCDWriteData(TWord usData)//ok
{
	TWord wPreamble	= 0x0000;

	SPI_Enable;
	
	LCDWaitForReady();
	SPI_Send_byte(wPreamble);		
	
	LCDWaitForReady();
	SPI_Send_byte(usData);	
	
	delay_nop();
	
	SPI_Disable;
}

void LCDWriteNData(TWord* pwBuf, TDWord ulSizeWordCnt)
{
	TWord wPreamble	= 0x0000;
	TWord i;

	SPI_Enable;
	
	LCDWaitForReady();
	SPI_Send_byte(wPreamble);		
	
	LCDWaitForReady();
	//Send Data
	for(i=0;i<ulSizeWordCnt;i++)
	{
			SPI_Send_byte(pwBuf[i]);
	}
	
	delay_nop();
	
	SPI_Disable;
}  

//-----------------------------------------------------------
//Host controller function 4   Read Data from host data Bus
//-----------------------------------------------------------
TWord LCDReadData()
{
	TWord wRData; 

	//set type and direction
	TWord wPreamble	= 0x1000;
	
	SPI_Enable;
	
	LCDWaitForReady();
	SPI_Send_byte(wPreamble);		
	
	LCDWaitForReady();
	SPI_Receive_byte();//Dummy
	
	LCDWaitForReady();
	//Read Data
	wRData = SPI_Receive_byte();
	
	SPI_Disable;
	
	return wRData;
}

//-------------------------------------------------------------------
//  Read Burst N words Data
//-------------------------------------------------------------------
void LCDReadNData(TWord* pwBuf, TDWord ulSizeWordCnt)
{
	TDWord i;
	
	//set type and direction
	TWord wPreamble	= 0x1000;

	SPI_Enable;
	
	//Send Preamble before reading data
	LCDWaitForReady();
	SPI_Send_byte(wPreamble);		

	LCDWaitForReady();
	SPI_Receive_byte();//Dummy
	
	LCDWaitForReady();
	for(i=0;i<ulSizeWordCnt;i++)
	{
		//Read Data
		pwBuf[i]= SPI_Receive_byte();
	}
	
	SPI_Disable;
}

#endif

#if defined (IT8951_Interface_I2C)
   
#define SET_SDA()     GPIO_SetBits(CT_SDA_GPIO_PORT,CT_SDA_GPIO_PIN)    
#define CLR_SDA()     GPIO_ResetBits(CT_SDA_GPIO_PORT,CT_SDA_GPIO_PIN)  

#define SET_CLK()     GPIO_SetBits(CT_SCL_GPIO_PORT,CT_SCL_GPIO_PIN)    
#define CLR_CLK()     GPIO_ResetBits(CT_SCL_GPIO_PORT,CT_SCL_GPIO_PIN)

#define CT_READ_SDA   GPIO_ReadInputDataBit(CT_SDA_GPIO_PORT,CT_SDA_GPIO_PIN)
#define CT_IIC_SDA     

#define CT_SDA_CLK    	   RCC_AHB1Periph_GPIOB
#define CT_SDA_GPIO_PORT   GPIOB
#define CT_SDA_GPIO_PIN    GPIO_Pin_7

#define CT_SCL_CLK    	   RCC_AHB1Periph_GPIOB
#define CT_SCL_GPIO_PORT   GPIOB
#define CT_SCL_GPIO_PIN    GPIO_Pin_6

void InterfaceInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd(CT_SDA_CLK | CT_SCL_CLK | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = CT_SCL_GPIO_PIN;
	GPIO_Init(CT_SCL_GPIO_PORT, &GPIO_InitStructure);	
	
	CLR_CLK();
	
	GPIO_InitStructure.GPIO_Pin = CT_SDA_GPIO_PIN;
	GPIO_Init(CT_SDA_GPIO_PORT, &GPIO_InitStructure);	
	
	SET_SDA();
	
	delay_ms(100);
	
	SET_CLK();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOC,GPIO_Pin_5);
	delay_ms(100);
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	delay_ms(20);
	GPIO_SetBits(GPIOC,GPIO_Pin_5);
	delay_ms(100);
}

void delay_nop()
{
	TWord i;
	for (i=0;i<1000;i++);	
}

void CT_SDA_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(CT_SDA_CLK , ENABLE);
	/* Configure PF6 PF7 PF8 PF9 in output pushpull mode */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = CT_SDA_GPIO_PIN;
	GPIO_Init(CT_SDA_GPIO_PORT, &GPIO_InitStructure);	
}

void CT_SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(CT_SDA_CLK , ENABLE);
	/* Configure PF6 PF7 PF8 PF9 in output pushpull mode */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = CT_SDA_GPIO_PIN;
	GPIO_Init(CT_SDA_GPIO_PORT, &GPIO_InitStructure);	
}

//产生I2C起始信号
void CT_I2C_Start(void)
{
	CT_SDA_OUT();     //sda线输出
	SET_SDA();	  	  
	SET_CLK();
	delay_us(1);
 	CLR_SDA();//START:when CLK is high,DATA change form high to low 
	delay_us(1);
	CLR_CLK();//钳住I2C总线，准备发送或接收数据 
}	  

//产生I2C停止信号
void CT_I2C_Stop(void)
{
	CT_SDA_OUT();//sda线输出
	
	CLR_CLK();
	CLR_SDA();//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	SET_CLK();
	delay_us(1);	
	SET_SDA();//发送I2C总线结束信号
	delay_us(1);		
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 CT_I2C_Wait_Ack(void)
{
	u16 ucErrTime=0;
	CT_SDA_IN(); 
	SET_CLK();
	//delay_us(1);	
	while(CT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_I2C_Stop();
			return 1;
		}
	}
	CLR_CLK();//时钟输出0 	   
	return 0;  
} 

//产生ACK应答
void CT_I2C_Ack(void)
{
	CLR_CLK();
	CT_SDA_OUT();
	CLR_SDA();
	delay_us(1);
	SET_CLK();
	delay_us(1);
	CLR_CLK();
}

//不产生ACK应答		    
void CT_I2C_NAck(void)
{
	CLR_CLK();
	CT_SDA_OUT();
	SET_SDA();
	delay_us(1);
	SET_CLK();
	delay_us(1);
	CLR_CLK();
}		

//I2C发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void CT_I2C_Send_Byte(u8 txd)
{                        
	u8 t;   
	CT_SDA_OUT(); 	    
	CLR_CLK();//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{              
		if((txd&0x80)>>7)
			SET_SDA();
		else	
			CLR_SDA();

		txd<<=1; 	
		delay_us(1);
		SET_CLK();
		delay_us(1);
		CLR_CLK();	
		delay_us(1);
	}	 
} 	    

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 CT_I2C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
 	CT_SDA_IN();//SDA设置为输入
	
	for(i=0;i<8;i++ )
	{
    CLR_CLK();
		delay_us(1);
		SET_CLK();
    receive<<=1;
    if(CT_READ_SDA)receive++;
		delay_us(1);
	}
	
	if (!ack)CT_I2C_NAck();//发送nACK
	else CT_I2C_Ack(); //发送ACK   

 	return receive;
}

#define I2C_SLAVE_ID	         	 		0x46 //7-bits S1
//#define I2C_SLAVE_ID	         	 	0x35 //7-bits S2
#define I80_I2C_CMD_TYPE_CMD     		0x00 //Preamble of Command
#define I80_I2C_CMD_TYPE_DATA    		0x80 //Preamble of Data

void LCDWaitForReady()
{
	TByte ulData = READ_RDY;
	while(ulData == 0)
	{
		ulData = READ_RDY;
	}
}

void i2c_master_tx(TByte ucSlaveID, TByte Premable, TByte ulSize, TByte* pWBuf)
{
	int i;
	
	CT_I2C_Start();
	
	CT_I2C_Send_Byte(ucSlaveID << 1 | 0);
	CT_I2C_Wait_Ack();
	
	CT_I2C_Send_Byte(Premable);
	CT_I2C_Wait_Ack(); 	 	
	
	for(i=0;i<ulSize;i++)
	{
		CT_I2C_Send_Byte(pWBuf[i]);
		CT_I2C_Wait_Ack(); 	
	}	
	
	CT_I2C_Stop();
}

//------------------------------------------------------
//Pseudo Code - Basic i2c Read function
//------------------------------------------------------
void i2c_master_rx(TByte ucSlaveID, TByte Premable, TByte ulSize, TByte* pRBuf)
{
	int i;
	
	CT_I2C_Start();

	CT_I2C_Send_Byte(ucSlaveID << 1 | 0);
	CT_I2C_Wait_Ack(); 

	CT_I2C_Send_Byte(Premable);
	CT_I2C_Wait_Ack();

	CT_I2C_Start();
	
	CT_I2C_Send_Byte(ucSlaveID << 1 | 1);
	CT_I2C_Wait_Ack();
	
 	pRBuf[0] = CT_I2C_Read_Byte(1);
 	pRBuf[0] = CT_I2C_Read_Byte(1);

	for(i=0;i<ulSize;i++)
	{
 			pRBuf[i] = CT_I2C_Read_Byte(i==(ulSize-1)?0:1);
			delay_nop();
	}

	CT_I2C_Stop();
}

//-----------------------------------------------------------------
//Host controller function 2   Write command code to host data Bus
//-----------------------------------------------------------------
void LCDWriteCmdCode(TWord usCmdCode)
{
	LCDWaitForReady();

	i2c_master_tx(I2C_SLAVE_ID, I80_I2C_CMD_TYPE_CMD, 2, (TByte*)&usCmdCode); 
}

//-----------------------------------------------------------
//Host controller function 3   Write Data to host data Bus
//-----------------------------------------------------------
void LCDWriteData(TWord usData)
{
	LCDWaitForReady();
	
	//Send Preamble and Data
  i2c_master_tx(I2C_SLAVE_ID, I80_I2C_CMD_TYPE_DATA, 2, (TByte*)&usData); 
}

void LCDWriteNData(TWord* pwBuf, TDWord ulSizeWordCnt)
{
	//Send Data
	LCDWaitForReady();
	//Send Preamble and Data
  i2c_master_tx(I2C_SLAVE_ID, I80_I2C_CMD_TYPE_DATA, ulSizeWordCnt*2, (TByte*)pwBuf); 
}  

//-----------------------------------------------------------
//Host controller function 4    Read Data from host data Bus
//-----------------------------------------------------------
TWord LCDReadData()
{
	TWord wRData; 

	LCDWaitForReady();

	//Read 1 16-bits Data
	i2c_master_rx(I2C_SLAVE_ID, I80_I2C_CMD_TYPE_DATA, 2, (TByte*)&wRData); 

	return wRData;
}

//-------------------------------------------------------------------
//  Read Burst N words Data
//-------------------------------------------------------------------
void LCDReadNData(TWord* pwBuf, TDWord ulSizeWordCnt)
{
	LCDWaitForReady();

	i2c_master_rx(I2C_SLAVE_ID, I80_I2C_CMD_TYPE_DATA, ulSizeWordCnt, (TByte *)pwBuf); 
}


#endif

//-----------------------------------------------------------
//Host controller function 5  Write command to host data Bus with aruments
//-----------------------------------------------------------
void LCDSendCmdArg(TWord usCmdCode,TWord* pArg, TWord usNumArg)
{
     TWord i;
     //Send Cmd code
     LCDWriteCmdCode(usCmdCode);
     //Send Data
     for(i=0;i<usNumArg;i++)
     {
         LCDWriteData(pArg[i]);
     }
}

//-----------------------------------------------------------
//Host Cmd 1  SYS_RUN
//-----------------------------------------------------------
void IT8951SystemRun()
{
    LCDWriteCmdCode(IT8951_TCON_SYS_RUN);
}

//-----------------------------------------------------------
//Host Cmd 2 - STANDBY
//-----------------------------------------------------------
void IT8951StandBy()
{
    LCDWriteCmdCode(IT8951_TCON_STANDBY);
}

//-----------------------------------------------------------
//Host Cmd 3 - SLEEP
//-----------------------------------------------------------
void IT8951Sleep()
{
    LCDWriteCmdCode(IT8951_TCON_SLEEP);
}

//-----------------------------------------------------------
//Host Cmd 4 - REG_RD
//-----------------------------------------------------------
TWord IT8951ReadReg(TWord usRegAddr)
{
	TWord usData;
	//----------I80 Mode-------------
	//Send Cmd and Register Address
	LCDWriteCmdCode(IT8951_TCON_REG_RD);
	LCDWriteData(usRegAddr);
	//Read data from Host Data bus
	usData = LCDReadData();
	return usData;
}

//-----------------------------------------------------------
//Host Cmd 5 - REG_WR
//-----------------------------------------------------------
void IT8951WriteReg(TWord usRegAddr,TWord usValue)
{
	//I80 Mode
	//Send Cmd , Register Address and Write Value
	LCDWriteCmdCode(IT8951_TCON_REG_WR);
	LCDWriteData(usRegAddr);
	LCDWriteData(usValue);
}

//-----------------------------------------------------------
//Host Cmd 6 - MEM_BST_RD_T
//-----------------------------------------------------------
void IT8951MemBurstReadTrigger(TDWord ulMemAddr , TDWord ulReadSize)
{
    TWord usArg[4];
    //Setting Arguments for Memory Burst Read
    usArg[0] = (TWord)(ulMemAddr & 0x0000FFFF); //addr[15:0]
    usArg[1] = (TWord)( (ulMemAddr >> 16) & 0x0000FFFF ); //addr[25:16]
    usArg[2] = (TWord)(ulReadSize & 0x0000FFFF); //Cnt[15:0]
    usArg[3] = (TWord)( (ulReadSize >> 16) & 0x0000FFFF ); //Cnt[25:16]
    //Send Cmd and Arg
    LCDSendCmdArg(IT8951_TCON_MEM_BST_RD_T , usArg , 4);
}

//-----------------------------------------------------------
//Host Cmd 7 - MEM_BST_RD_S
//-----------------------------------------------------------
void IT8951MemBurstReadStart()
{
    LCDWriteCmdCode(IT8951_TCON_MEM_BST_RD_S);
}

//-----------------------------------------------------------
//Host Cmd 8 - MEM_BST_WR
//-----------------------------------------------------------
void IT8951MemBurstWrite(TDWord ulMemAddr , TDWord ulWriteSize)
{
    TWord usArg[4];
    //Setting Arguments for Memory Burst Write
    usArg[0] = (TWord)(ulMemAddr & 0x0000FFFF); //addr[15:0]
    usArg[1] = (TWord)( (ulMemAddr >> 16) & 0x0000FFFF ); //addr[25:16]
    usArg[2] = (TWord)(ulWriteSize & 0x0000FFFF); //Cnt[15:0]
    usArg[3] = (TWord)( (ulWriteSize >> 16) & 0x0000FFFF ); //Cnt[25:16]
    //Send Cmd and Arg
    LCDSendCmdArg(IT8951_TCON_MEM_BST_WR , usArg , 4);
}

//-----------------------------------------------------------
//Host Cmd 9 - MEM_BST_END
//-----------------------------------------------------------
void IT8951MemBurstEnd(void)
{
    LCDWriteCmdCode(IT8951_TCON_MEM_BST_END);
}

//-----------------------------------------------------------
//Example of Memory Burst Write
//-----------------------------------------------------------
// ****************************************************************************************
// Function name: IT8951MemBurstWriteProc( )
//
// Description:
//   IT8951 Burst Write procedure
//      
// Arguments:
//      TDWord ulMemAddr: IT8951 Memory Target Address
//      TDWord ulWriteSize: Write Size (Unit: Word)
//      TByte* pDestBuf - Buffer of Sent data
// Return Values:
//   NULL.
// Note:
//
// ****************************************************************************************
void IT8951MemBurstWriteProc(TDWord ulMemAddr , TDWord ulWriteSize, TWord* pSrcBuf )
{
    
    TDWord i;
 
    //Send Burst Write Start Cmd and Args
    IT8951MemBurstWrite(ulMemAddr , ulWriteSize);
 
    //Burst Write Data
    for(i=0;i<ulWriteSize;i++)
    {
        LCDWriteData(pSrcBuf[i]);
    }
 
    //Send Burst End Cmd
    IT8951MemBurstEnd();
}

// ****************************************************************************************
// Function name: IT8951MemBurstReadProc( )
//
// Description:
//   IT8951 Burst Read procedure
//      
// Arguments:
//      TDWord ulMemAddr: IT8951 Read Memory Address
//      TDWord ulReadSize: Read Size (Unit: Word)
//      TByte* pDestBuf - Buffer for storing Read data
// Return Values:
//   NULL.
// Note:
//
// ****************************************************************************************
void IT8951MemBurstReadProc(TDWord ulMemAddr , TDWord ulReadSize, TWord* pDestBuf )
{
		#ifndef IT8951_Interface_SPI
    TDWord i;
		#endif
    //Send Burst Read Start Cmd and Args
    IT8951MemBurstReadTrigger(ulMemAddr , ulReadSize);
          
    //Burst Read Fire
    IT8951MemBurstReadStart();
    
    #ifdef IT8951_Interface_SPI
    
    //Burst Read Request for SPI interface only
    LCDReadNData(pDestBuf, ulReadSize);
    
    #else
    //Burst Read Data
    for(i=0;i<ulReadSize;i++)
    {
        pDestBuf[i] = LCDReadData();        
    }
    
    #endif

    //Send Burst End Cmd
    IT8951MemBurstEnd(); //the same with IT8951MemBurstEnd()
}

//-----------------------------------------------------------
//Host Cmd 10 - LD_IMG
//-----------------------------------------------------------
void IT8951LoadImgStart(IT8951LdImgInfo* pstLdImgInfo)
{
    TWord usArg;
    //Setting Argument for Load image start
    usArg = (pstLdImgInfo->usEndianType << 8 )
    |(pstLdImgInfo->usPixelFormat << 4)
    |(pstLdImgInfo->usRotate);
    //Send Cmd
    LCDWriteCmdCode(IT8951_TCON_LD_IMG);
    //Send Arg
    LCDWriteData(usArg);
}

//-----------------------------------------------------------
//Host Cmd 11 - LD_IMG_AREA
//-----------------------------------------------------------
void IT8951LoadImgAreaStart(IT8951LdImgInfo* pstLdImgInfo ,IT8951AreaImgInfo* pstAreaImgInfo)
{
    TWord usArg[5];
    //Setting Argument for Load image start
    usArg[0] = (pstLdImgInfo->usEndianType << 8 )
    |(pstLdImgInfo->usPixelFormat << 4)
    |(pstLdImgInfo->usRotate);
    usArg[1] = pstAreaImgInfo->usX;
    usArg[2] = pstAreaImgInfo->usY;
    usArg[3] = pstAreaImgInfo->usWidth;
    usArg[4] = pstAreaImgInfo->usHeight;
    //Send Cmd and Args
    LCDSendCmdArg(IT8951_TCON_LD_IMG_AREA , usArg , 5);
}

//-----------------------------------------------------------
//Host Cmd 12 - LD_IMG_END
//-----------------------------------------------------------
void IT8951LoadImgEnd(void)
{
    LCDWriteCmdCode(IT8951_TCON_LD_IMG_END);
}

//Global structures and variables
I80IT8951DevInfo gstI80DevInfo;
TByte* gpFrameBuf;
// TByte FrameBuf[59520];
TDWord gulImgBufAddr; //IT8951 Image buffer address

void GetIT8951SystemInfo(void* pBuf)
{
	TWord* pusWord = (TWord*)pBuf;
	I80IT8951DevInfo* pstDevInfo;
	
	#ifdef IT8951_Interface_I80
	TDWord i;	
	#endif

	LCDWriteCmdCode(USDEF_I80_CMD_GET_DEV_INFO);
 
	#if defined(IT8951_Interface_SPI) || defined(IT8951_Interface_I2C)
	//Burst Read Request for SPI interface only
	LCDReadNData(pusWord, sizeof(I80IT8951DevInfo));//Polling HRDY for each words(2-bytes) if possible
	#else
	//I80 interface - Single Read available
	for(i=0;i<sizeof(I80IT8951DevInfo)/2;i++)
	{
			pusWord[i] = LCDReadData();
	}
	#endif
	
	//Show Device information of IT8951
	pstDevInfo = (I80IT8951DevInfo*)pBuf;
	printf("Panel(W,H) = (%d,%d)\r\n",
	pstDevInfo->usPanelW, pstDevInfo->usPanelH );
	printf("Image Buffer Address = %X\r\n",
	pstDevInfo->usImgBufAddrL | (pstDevInfo->usImgBufAddrH << 16));
	//Show Firmware and LUT Version
	printf("FW Version = %s\r\n", (TByte*)pstDevInfo->usFWVersion);
	printf("LUT Version = %s\r\n", (TByte*)pstDevInfo->usLUTVersion);
}

//-----------------------------------------------------------
//Initial function 2  Set Image buffer base address
//-----------------------------------------------------------
void IT8951SetImgBufBaseAddr(TDWord ulImgBufAddr)
{
	TWord usWordH = (TWord)((ulImgBufAddr >> 16) & 0x0000FFFF);
	TWord usWordL = (TWord)( ulImgBufAddr & 0x0000FFFF);
	//Write LISAR Reg
	IT8951WriteReg(LISAR + 2 ,usWordH);
	IT8951WriteReg(LISAR ,usWordL);
}

//-----------------------------------------------------
// 3.6. Display Functions
//-----------------------------------------------------

//-----------------------------------------------------------
//Display function 1 - Wait for LUT Engine Finish
//                     Polling Display Engine Ready by LUTNo
//-----------------------------------------------------------
void IT8951WaitForDisplayReady()
{
	//Check IT8951 Register LUTAFSR => NonZero  Busy, 0 - Free
	while(IT8951ReadReg(LUTAFSR));
}

//-----------------------------------------------------------
//Display function 2  Load Image Area process
//-----------------------------------------------------------
void IT8951HostAreaPackedPixelWrite(IT8951LdImgInfo* pstLdImgInfo,IT8951AreaImgInfo* pstAreaImgInfo)
{
	TDWord i,j;
	//Source buffer address of Host
	TWord* pusFrameBuf = (TWord*)pstLdImgInfo->ulStartFBAddr;

	//Set Image buffer(IT8951) Base address
	IT8951SetImgBufBaseAddr(pstLdImgInfo->ulImgBufBaseAddr);
	//Send Load Image start Cmd
	IT8951LoadImgAreaStart(pstLdImgInfo , pstAreaImgInfo);
	printf("---IT8951 Host Area Packed Pixel Write begin---\r\n");
	//Host Write Data
	for(j=0;j< pstAreaImgInfo->usHeight;j++)
	{
		 for(i=0;i< pstAreaImgInfo->usWidth/2;i++)
			{
					//Write a Word(2-Bytes) for each time
					LCDWriteData(*pusFrameBuf);
					pusFrameBuf++;
			}
	}
	printf("---IT8951 Host Area Packed Pixel Write end---\r\n");
	//Send Load Img End Command
	IT8951LoadImgEnd();
}

//-----------------------------------------------------------
//Display functions 3 - Application for Display panel Area
//-----------------------------------------------------------
void IT8951DisplayArea(TWord usX, TWord usY, TWord usW, TWord usH, TWord usDpyMode)
{
	//Send I80 Display Command (User defined command of IT8951)
	LCDWriteCmdCode(USDEF_I80_CMD_DPY_AREA); //0x0034
	//Write arguments
	LCDWriteData(usX);
	LCDWriteData(usY);
	LCDWriteData(usW);
	LCDWriteData(usH);
	LCDWriteData(usDpyMode);
}

//Display Area with bitmap on EPD
//-----------------------------------------------------------
// Display Function 4 - for Display Area for 1-bpp mode format
//   the bitmap(1bpp) mode will be enable when Display
//   and restore to Default setting (disable) after displaying finished
//-----------------------------------------------------------
void IT8951DisplayArea1bpp(TWord usX, TWord usY, TWord usW, TWord usH, TWord usDpyMode, TByte ucBGGrayVal, TByte ucFGGrayVal)
{
    //Set Display mode to 1 bpp mode - Set 0x18001138 Bit[18](0x1800113A Bit[2])to 1
    IT8951WriteReg(UP1SR+2, IT8951ReadReg(UP1SR+2) | (1<<2));
    
    //Set BitMap color table 0 and 1 , => Set Register[0x18001250]:
    //Bit[7:0]: ForeGround Color(G0~G15)  for 1
    //Bit[15:8]:Background Color(G0~G15)  for 0
    IT8951WriteReg(BGVR, (ucBGGrayVal<<8) | ucFGGrayVal);
    
    //Display
    IT8951DisplayArea( usX, usY, usW, usH, usDpyMode);
    IT8951WaitForDisplayReady();
    
    //Restore to normal mode
    IT8951WriteReg(UP1SR+2, IT8951ReadReg(UP1SR+2) & ~(1<<2));
}

//-------------------------------------------------------------------------------------------------------------
// 	Command - 0x0037 for Display Base addr by User 
//  TDWord ulDpyBufAddr - Host programmer need to indicate the Image buffer address of IT8951
//                                         In current case, there is only one image buffer in IT8951 so far.
//                                         So Please set the Image buffer address you got  in initial stage.
//                                         (gulImgBufAddr by Get device information 0x0302 command)
//
//-------------------------------------------------------------------------------------------------------------
void IT8951DisplayAreaBuf(TWord usX, TWord usY, TWord usW, TWord usH, TWord usDpyMode, TDWord ulDpyBufAddr)
{
    //Send I80 Display Command (User defined command of IT8951)
    LCDWriteCmdCode(USDEF_I80_CMD_DPY_BUF_AREA); //0x0037
    
    //Write arguments
    LCDWriteData(usX);
    LCDWriteData(usY);
    LCDWriteData(usW);
    LCDWriteData(usH);
    LCDWriteData(usDpyMode);
    LCDWriteData((TWord)ulDpyBufAddr);       //Display Buffer Base address[15:0]
    LCDWriteData((TWord)(ulDpyBufAddr>>16)); //Display Buffer Base address[26:16]
}

//-----------------------------------------------------------
//Test function 1 Software Initial flow for testing
//-----------------------------------------------------------
void HostInit()
{
	//Get Device Info
	GetIT8951SystemInfo(&gstI80DevInfo);
	//Host Frame Buffer allocation
	gpFrameBuf = (TByte*)0xD0000000;
	//Get Image Buffer Address of IT8951
	gulImgBufAddr = gstI80DevInfo.usImgBufAddrL | (gstI80DevInfo.usImgBufAddrH << 16);
	
	//Set to Enable I80 Packed mode
	IT8951WriteReg(I80CPCR, 0x0001);
}

//-----------------------------------------------------------
//Test function 2  Example of Display Flow
//-----------------------------------------------------------
void IT8951DisplayExample()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;
	
	//Host Initial
	HostInit(); //Test Function 1
	//Prepare image
	//Write pixel 0xF0(White) to Frame Buffer
	memset(gpFrameBuf, 0xF0, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);
	
	//Check TCon is free ? Wait TCon Ready (optional)
	IT8951WaitForDisplayReady();
	
	//--------------------------------------------------------------------------------------------
	//      initial display - Display white only
	//--------------------------------------------------------------------------------------------
	//Load Image and Display
	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (TDWord)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;
	
	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2

	//Display Area  (x,y,w,h) with mode 0 for initial White to clear Panel
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 0);
	//--------------------------------------------------------------------------------------------
	//      Regular display - Display Any Gray colors with Mode 2 or others
	//--------------------------------------------------------------------------------------------
	//Preparing buffer to All black (8 bpp image)
	//or you can create your image pattern here..
	memset(gpFrameBuf, 0x00, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);
	 
	IT8951WaitForDisplayReady();
	
	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (TDWord)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP; 
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;
	
	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform 
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);
}

void IT8951DisplayExample2()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;
	uint16_t width = gstI80DevInfo.usPanelW;
	uint16_t high = gstI80DevInfo.usPanelH/16;
	
	//--------------------------------------------------------------------------------------------
	//      Regular display - Display Any Gray colors with Mode 2 or others
	//--------------------------------------------------------------------------------------------
	//Preparing buffer to All black (8 bpp image)
	//or you can create your image pattern here..
	memset(gpFrameBuf                 ,  0x00, width * high * 1);
	memset(gpFrameBuf+width * high * 1,  0x11, width * high * 1);
	memset(gpFrameBuf+width * high * 2,  0x22, width * high * 1);
	memset(gpFrameBuf+width * high * 3,  0x33, width * high * 1);
	memset(gpFrameBuf+width * high * 4,  0x44, width * high * 1);
	memset(gpFrameBuf+width * high * 5,  0x55, width * high * 1);
	memset(gpFrameBuf+width * high * 6,  0x66, width * high * 1);
	memset(gpFrameBuf+width * high * 7,  0x77, width * high * 1);
	memset(gpFrameBuf+width * high * 8,  0x88, width * high * 1);
	memset(gpFrameBuf+width * high * 9,  0x99, width * high * 1);
	memset(gpFrameBuf+width * high * 10, 0xaa, width * high * 1);
	memset(gpFrameBuf+width * high * 11, 0xbb, width * high * 1);
	memset(gpFrameBuf+width * high * 12, 0xcc, width * high * 1);
	memset(gpFrameBuf+width * high * 13, 0xdd, width * high * 1);
	memset(gpFrameBuf+width * high * 14, 0xee, width * high * 1);
	memset(gpFrameBuf+width * high * 15, 0xff, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH - width * high * 15);

	IT8951WaitForDisplayReady();
	
	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (TDWord)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP; 
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;
	
	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform 
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);
}

extern const unsigned char pic[];
extern const unsigned char zoo_800_600[];
extern const unsigned char pic_1872x1404_2[];

void IT8951DisplayExample3()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;
	TDWord i;
	memset(gpFrameBuf, 0x00, 800*600*1);
	for (i=0; i<800*600; i++)
	{
//		gpFrameBuf[i] = pic[i];					//9.7inch e-paper 1200*825
		gpFrameBuf[i] = zoo_800_600[i]; 	//6inch e-paper    800*600
	}

	IT8951WaitForDisplayReady();
	
	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (TDWord)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP; 
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = 800;
	stAreaImgInfo.usHeight = 600;
	
	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform 
	IT8951DisplayArea(0,0, 800, 600, 2);
}

//-----------------------------------------------------------
// Load 1bpp image flow (must display with IT8951DisplayArea1bpp()
//-----------------------------------------------------------

void IT8951Load1bppImage(TByte* p1bppImgBuf, TWord usX, TWord usY, TWord usW, TWord usH)
{
	IT8951LdImgInfo stLdImgInfo;
    IT8951AreaImgInfo stAreaImgInfo;
	
    //Setting Load image information
    stLdImgInfo.ulStartFBAddr    = (TDWord) p1bppImgBuf;
    stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
    stLdImgInfo.usPixelFormat    = IT8951_8BPP; //we use 8bpp because IT8951 dose not support 1bpp mode for load imageso we use Load 8bpp mode ,but the transfer size needs to be reduced to Size/8
    stLdImgInfo.usRotate         = IT8951_ROTATE_0;
    stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
    //Set Load Area
    stAreaImgInfo.usX      = usX/8;
    stAreaImgInfo.usY      = usY;
    stAreaImgInfo.usWidth  = usW/8;//1bpp, Chaning Transfer size setting to 1/8X of 8bpp mode 
    stAreaImgInfo.usHeight = usH;
    printf("IT8951HostAreaPackedPixelWrite [wait]\n\r");
    //Load Image from Host to IT8951 Image Buffer
    IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
}

//-----------------------------------------------------------
//Test function 3 - Example of Display 1bpp Flow
//-----------------------------------------------------------
void IT8951Display1bppExample()
{
    IT8951AreaImgInfo stAreaImgInfo;
    
    //Host Initial
    HostInit(); //Test Function 1
    //Prepare image
    //Write pixel 0x00(Black) to Frame Buffer
    //or you can create your image pattern here..
    memset(gpFrameBuf, 0x00, (gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH)/8);//Host Frame Buffer(Source)
    
    //Check TCon is free ? Wait TCon Ready (optional)
    IT8951WaitForDisplayReady();
    
    //Load Image and Display
    //Set Load Area
    stAreaImgInfo.usX      = 0;
    stAreaImgInfo.usY      = 0;
    stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
    stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;
    //Load Image from Host to IT8951 Image Buffer
    IT8951Load1bppImage(gpFrameBuf, stAreaImgInfo.usX, stAreaImgInfo.usY, stAreaImgInfo.usWidth, stAreaImgInfo.usHeight);//Display function 4, Arg
    
    //Display Area - (x,y,w,h) with mode 2 for Gray Scale
    //e.g. if we want to set b0(Background color) for Black-0x00 , Set b1(Foreground) for White-0xFF
    IT8951DisplayArea1bpp(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 0, 0x00, 0xFF);
}

void IT8951Display1bppExample2()
{
   IT8951AreaImgInfo stAreaImgInfo;
    
    //Host Initial
    HostInit(); //Test Function 1
    //Prepare image
    //Write pixel 0x00(Black) to Frame Buffer
    //or you can create your image pattern here..
    memset(gpFrameBuf, 0xff, (gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH)/8);//Host Frame Buffer(Source)
    
    //Check TCon is free ? Wait TCon Ready (optional)
    IT8951WaitForDisplayReady();
    
    //Load Image and Display
    //Set Load Area
    stAreaImgInfo.usX      = 0;
    stAreaImgInfo.usY      = 0;
    stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
    stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;
    //Load Image from Host to IT8951 Image Buffer
    IT8951Load1bppImage(gpFrameBuf, stAreaImgInfo.usX, stAreaImgInfo.usY, stAreaImgInfo.usWidth, stAreaImgInfo.usHeight);//Display function 4, Arg
    
    //Display Area - (x,y,w,h) with mode 2 for Gray Scale
    //e.g. if we want to set b0(Background color) for Black-0x00 , Set b1(Foreground) for White-0xFF
    IT8951DisplayArea1bpp(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 0, 0x00, 0xFF);
}

//-----------------------------------------------------------
//Test function 2  Example of Display Flow
//-----------------------------------------------------------
void IT8951DisplayClear()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;
	
	
	//--------------------------------------------------------------------------------------------
	//      Regular display - Display Any Gray colors with Mode 2 or others
	//--------------------------------------------------------------------------------------------
	//Preparing buffer to All black (8 bpp image)
	//or you can create your image pattern here..
	memset(gpFrameBuf, 0x00, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);
	 
	IT8951WaitForDisplayReady();
	
	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (TDWord)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP; 
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;
	
	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform 
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);
	
	
	//--------------------------------------------------------------------------------------------
	//      initial display - Display white only
	//--------------------------------------------------------------------------------------------
	//Load Image and Display
	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (TDWord)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;
	
	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 0 for initial White to clear Panel
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 0);
}
