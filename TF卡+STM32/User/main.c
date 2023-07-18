/************************************************************************************
//�����������������ҡ����Ա�����: https://shop149406681.taobao.com/?spm=a1z10.1-c.0.0.64cb457fKzgKea 
//�Ա����������ؼ��ʣ�K_A39_001
//  �� �� ��   : v1.0
//  ��    ��   : ����������
//  ����Ⱥ :			 1041406448�����������������ע���棩
//    CSDN  ��������������Դ�����б����н���˵����   :��Ƭ��������˹�(�����ؼ��ʣ�K_A39_001)
//    BILIBILI����ʾ��Ƶ�� :����������(�����ؼ��ʣ�K_A39_001)
//  ��������   : 2021-6-21
//  ����޸�   : 2021-11-21
//  ��������   : TF��ģ����Գ���
//  ��������   : STM32F103C8T6   ����8M  ϵͳʱ��72M
����
TF��ģ��-------------------------------STM32F103C8T6
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
						WriteBuffer[i]=0xC4FA;//��
				}
				else
				{
						WriteBuffer[i]=0xBAC3;//��
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
						Usart_SendByte(DEBUG_USARTx, ReadBuffer[i]>>8); //�򴮿ڷ��͸�8λ���� 
						Usart_SendByte(DEBUG_USARTx, ReadBuffer[i] ); 
					

				}
				Usart_SendString(DEBUG_USARTx,"\r\n");
				
				DelayMs(300);
		}
}
