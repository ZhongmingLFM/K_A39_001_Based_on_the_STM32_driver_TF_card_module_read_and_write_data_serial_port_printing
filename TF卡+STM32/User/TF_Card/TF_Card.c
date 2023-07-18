/************************************************************************************
//（店名“众明工作室”）淘宝链接: https://shop149406681.taobao.com/?spm=a1z10.1-c.0.0.64cb457fKzgKea 
//淘宝店内搜索关键词：K_A39_001
//  版 本 号   : v1.0
//  作    者   : 众明工作室
//  交流群 :			 1041406448（其它更多资料请关注公告）
//    CSDN  （基本参数，资源更新列表，所有接线说明）   :单片机代码搬运工(搜索关键词：K_A39_001)
//    BILIBILI（演示视频） :众明工作室(搜索关键词：K_A39_001)
//  生成日期   : 2021-6-21
//  最近修改   : 2021-11-21
//  功能描述   : TF卡模块测试程序
//  测试条件   : STM32F103C8T6   晶振8M  系统时钟72M
接线
TF卡模块-------------------------------STM32F103C8T6
VCC------------------------------------3.3/5V
GND------------------------------------GND
CS-------------------------------------PB6 //
SCK------------------------------------PB7 //	
MISO-----------------------------------PB8 //	
MOSI-----------------------------------PB9 //	

*************************************************************************************/

#include "TF_Card.h"
#include "delay.h"

uint16_t  ReadBuffer[128];
uint16_t  WriteBuffer[128];
uint16_t BlockNR;
//********************************************
void SD_2Byte_Write(uint16_t IOData)
{
	uint8_t BitCounter;
		
	for (BitCounter=0;BitCounter<16;BitCounter++)
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		if(IOData&0x8000)//If the MSB of IOData is 1, then Do=1, else Do=0.
			SD_SPI_MOSI_1;//Do High
		else
			SD_SPI_MOSI_0;//Do Low
				
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
		
		IOData=IOData<<1;//Because the MSB is transmitted firstly, shift to next lower bit.
	}
}
//********************************************
void SD_Write(uint16_t IOData)
{
	uint8_t BitCounter;
	IOData=IOData<<8;
	
	for (BitCounter=0;BitCounter<8;BitCounter++)
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		if(IOData&0x8000)//If the MSB of IOData is 1, then Do=1, else Do=0.
			SD_SPI_MOSI_1;//Do High
		else
			SD_SPI_MOSI_0;//Do Low
				
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
		
		IOData=IOData<<1;//Because the MSB is transmitted firstly, shift to next lower bit.
	}
}
 
//********************************************
uint16_t SD_2Byte_Read(void)
{
	uint16_t Buffer;
	uint8_t BitCounter;
	Buffer=0;
	
	for (BitCounter=0;BitCounter<16;BitCounter++)
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
		Buffer=Buffer<<1;//Because the MSB is transmitted firstly, shift to next lower bit.
		                 //Because the LSB will be damaged, we can not put this line under next line.
		if(READ_SD_SPI_MISO)  
			Buffer++;//If SPI_Din=1 then the LSB_of_Buffer=1.		
	}
	
	return Buffer;
}

//********************************************
uint16_t SD_Read(void)
{
	uint16_t Buffer;
	uint8_t BitCounter;
	Buffer=0xffff;
	
	for (BitCounter=0;BitCounter<8;BitCounter++)
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
		Buffer=Buffer<<1;//Because the MSB is transmitted firstly, shift to next lower bit.
		                 //Because the LSB will be damaged, we can not put this line under next line.
		if(READ_SD_SPI_MISO)  
			Buffer++;//If SPI_Din=1 then the LSB_of_Buffer=1.		
	}
	
	return Buffer;
}
//********************************************
uint16_t SD_CMD_Write(uint16_t CMDIndex,uint16_t CMDArg,uint16_t ResType,uint16_t CSLowRSV)//ResType:Response Type, send 1 for R1; send 2 for R1b; send 3 for R2.
{	//There are 7 steps need to do.(marked by [1]-[7])
	uint16_t temp,Response,Response2,SD_CRC;
	Response2=0;
	//MaximumTimes=10;
	SD_CRC=0x0095;//0x0095 is only valid for CMD0
	if (CMDIndex!=0) SD_CRC=0x00ff;
	
	SPI_SD_CS_0;//[1] CS Low
	 
	SD_2Byte_Write(((CMDIndex|0x0040)<<8)+(CMDArg>>24));//[2] Transmit Command_Index & 1st Byte of Command_Argument.
	SD_2Byte_Write((CMDArg&0x00ffff00)>>8);				//[2] 2nd & 3rd Byte of Command_Argument
	SD_2Byte_Write(((CMDArg&0x000000ff)<<8)+SD_CRC);		//[2] 4th Byte of Command_Argument & CRC only for CMD0
	
	SD_SPI_MOSI_1;//[3] Do High
						//[3] Restore Do to High Level
	
	 for (temp=0;temp<8;temp++)//[4] Provide 8 extra clock after CMD
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	switch (ResType)//[5] wait response
	{
		case 1://R1
				{
					do
						Response=SD_Read();
					while (Response==0xffff);
					break;
				}
		case 2://R1b
				{
					do
						Response=SD_Read();
					while (Response==0xffff);//Read R1 firstly
					
					do
						Response2=SD_Read()-0xff00;
					while (Response2!=0);//Wait until the Busy_Signal_Token is non-zero
					break;	
				}
		case 3: Response=SD_2Byte_Read();break;//R2
	}
	
	if (CSLowRSV==0) SPI_SD_CS_1;//[6] CS High (if the CMD has data block response CS should be kept low)
	 
	 for (temp=0;temp<8;temp++)//[7] Provide 8 extra clock after card response
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	return Response;
}

//********************************************
uint16_t SD_Reset_Card(void)
{
	unsigned int temp;
	//MaximumTimes=10;
	
	for (temp=0;temp<80;temp++)//Send 74+ Clocks
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
		
	return SD_CMD_Write(0x0000,0x00000000,1,0);//Send CMD0
}

//********************************************

uint16_t  SD_Initiate_Card(void)//Polling the card after reset
{
	unsigned int temp,Response,MaximumTimes;
	MaximumTimes=50;
	
	for(temp=0;temp<MaximumTimes;temp++)
	{
		Response=SD_CMD_Write(0x0037,0x00000000,1,0);//Send CMD55
		Response=SD_CMD_Write(0x0029,0x00000000,1,0);//Send ACMD41
		if (Response==0xff00)
			temp=MaximumTimes;
	}

	return Response;
}

//********************************************

uint16_t SD_Get_CardInfo(void)//Read CSD register
{
	unsigned int temp,Response,MaximumTimes;
	MaximumTimes=50;
	
	for(temp=0;temp<MaximumTimes;temp++)
	{
		Response=SD_CMD_Write(9,0x00000000,1,1);//Send CMD9
		if (Response==0xff00)
			temp=MaximumTimes;
	}
	
	 for (temp=0;temp<8;temp++)//Provide 8 clock to romove the first byte of data response (0x00fe)
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	for (temp=0;temp<8;temp++) ReadBuffer[temp]=SD_2Byte_Read();//Get the CSD data
	
	for (temp=0;temp<16;temp++)//Provide 16 clock to remove the last 2 bytes of data response (CRC)
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	SPI_SD_CS_1;//CS_High()
	
	for (temp=0;temp<8;temp++)//Provide 8 extra clock after data response
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	BlockNR=((ReadBuffer[3]<<2)&0x0fff)+((ReadBuffer[4]>>14)&0x0003)+1;//Calcuate MULT
	BlockNR=BlockNR*(0x0002<<(((ReadBuffer[4]<<1)&0x0007)+((ReadBuffer[5]>>15)&0x0001)+1));//Calcuate Block_Number
	return Response;
}
//********************************************

uint16_t SD_Overall_Initiation(void)
{
	unsigned int Response,Response_2;
	Response=0x0000;
	Response_2=0xff00;
	
	SD_SPI_MOSI_1;//[1] Do High
						//[1] Do must be High when there is no transmition
	do
		Response=SD_Reset_Card();//[2] Send CMD0
	while (Response!=0xff01);
	
	if (Response!=0xff01) Response_2+=8;
	
	//Response=SD_CMD_Write(8,0x00000000,1,0);//Send CMD8
	
	Response=SD_Initiate_Card();//[3] Send CMD55+ACMD41 
	if (Response==0xff00)
		;
	else
		{
		Response_2+=4;
		;
		}
	
	do
		Response=SD_Get_CardInfo();//[4] Read CSD
	while (Response!=0xff00);
	if (Response==0xff01) Response_2+=2;
	
	return Response_2;
//	0000|0000||0000|0000 Response_2
//                  |||_CSD Fail
//                  ||__CMD55+ACMD41 Fail
//                  |___CMD0 Fail
}


//********************************************

uint16_t SD_Get_CardID(void)//Read CID register
{
	unsigned int temp,Response,MaximumTimes;
	MaximumTimes=10;
	
	for(temp=0;temp<MaximumTimes;temp++)
	{
		Response=SD_CMD_Write(10,0x00000000,1,1);//Send CMD9
		if (Response==0xff00)
			temp=MaximumTimes;
	}
	
	 for (temp=0;temp<8;temp++)//Provide 8 clock to romove the first byte of data response (0x00fe)
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	for (temp=0;temp<8;temp++) ReadBuffer[temp]=SD_2Byte_Read();//Get the CID data
	
	for (temp=0;temp<16;temp++)//Provide 16 clock to remove the last 2 bytes of data response (CRC)
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	SPI_SD_CS_1;//CS_High()
	
	for (temp=0;temp<8;temp++)//Provide 8 extra clock after data response
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	return Response;
}

//********************************************

uint16_t Read_Single_Block(uint32_t BlockAddress)
{
	unsigned int temp,Response,MaximumTimes;
	MaximumTimes=10;
	
	if (BlockAddress>BlockNR) return 0xff20;//whether BlockAddress out of range?
	
	for(temp=0;temp<MaximumTimes;temp++)
	{
		Response=SD_CMD_Write(17,BlockAddress,1,1);//Send CMD17
		if (Response==0xff00)
			temp=MaximumTimes;
	}
	
	while (SD_Read()!=0xfffe) {;}
	//这里为了使只有512byte的单片机能够读写SD卡，特意节省了RAM的使用量，每次读写只有两个重复的128byte
	//如果您使用的单片机拥有1K以上的RAM请将"%128"去掉	
	for (temp=0;temp<256;temp++) ReadBuffer[temp%128]=SD_2Byte_Read();//Get the readed data
	
	for (temp=0;temp<16;temp++)//Provide 16 clock to remove the last 2 bytes of data response (CRC)
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	SPI_SD_CS_1;//CS_High()
	
	for (temp=0;temp<8;temp++)//Provide 8 extra clock after data response
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	return Response;
}


//********************************************
uint16_t Write_Single_Block(uint32_t BlockAddress)
{
	unsigned int temp,Response,MaximumTimes;
	MaximumTimes=10;
	
	if (BlockAddress>BlockNR) return 0xff20;//whether BlockAddress out of range?
	
	for(temp=0;temp<MaximumTimes;temp++)
	{
		Response=SD_CMD_Write(24,BlockAddress,1,1);//Send CMD24
		if (Response==0xff00)
			temp=MaximumTimes;
	}
	
	for (temp=0;temp<8;temp++)//Provide 8 extra clock after CMD response
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	SD_Write(0x00fe);//Send Start Block Token
	//这里为了使只有512byte的单片机能够读写SD卡，特意节省了RAM的使用量，每次读写只有两个重复的128byte
	//如果您使用的单片机拥有1K以上的RAM请将"%128"去掉
	for (temp=0;temp<256;temp++) SD_2Byte_Write(WriteBuffer[temp%128]);//Data Block
	SD_2Byte_Write(0xffff);//Send 2 Bytes CRC
	
	Response=SD_Read();
	while (SD_Read()!=0xffff) {;}
	
	SPI_SD_CS_1;//CS_High()
	
	for (temp=0;temp<8;temp++)//Provide 8 extra clock after data response
	{
		SD_SPI_SCK_0;//CLK Low
		DelayUs(5);
		SD_SPI_SCK_1;//CLK High
		DelayUs(5);
	}
	
	return Response;
}






























































