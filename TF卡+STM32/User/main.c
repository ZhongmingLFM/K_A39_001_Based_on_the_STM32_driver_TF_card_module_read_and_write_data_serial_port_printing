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


#include "oled.h"
#include "delay.h"
#include "TF_Card.h"
#include "bsp_usart.h"




int main(void)
{
		uint16_t M_Response=0;
		uint16_t Data;
		uint8_t i;
		DelayInit();
//		OLED_Init();
//		OLED_Clear();
		USART_Config();
		SPI_SD_Init();
		for(i=0;i<128;i++)
		{
				if (i < 64)
				{
						WriteBuffer[i]=0xC4FA;//您
				}
				else
				{
						WriteBuffer[i]=0xBAC3;//好
				}
		}
		while(1)
		{
				M_Response=SD_Overall_Initiation();
				M_Response=SD_CMD_Write(16,512,1,0);
				Data=SD_Get_CardID();
				Write_Single_Block(0x0000);
				Read_Single_Block(0x0000); 

				for(i=0; i<128; i++)
				{ 	 
						Usart_SendByte(DEBUG_USARTx, ReadBuffer[i]>>8); //向串口发送高8位数据 
						Usart_SendByte(DEBUG_USARTx, ReadBuffer[i] ); 
					

				}
				Usart_SendString(DEBUG_USARTx,"\r\n");
				
				DelayMs(300);
		}
}
