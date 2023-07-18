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

#include "TF_Card_SPI.h"
#include "delay.h"


/**
  * @brief  SPI_SD���ų�ʼ��
  * @param  ��
  * @retval ��
  */
void SPI_SD_Init(void)
{
 
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ��SPI������ص�ʱ�� */
 	SD_SPI_ALL_APBxClock_FUN ( SD_SPI_ALL_CLK, ENABLE );
	
  /* ����SPI�� CS���ţ���ͨIO���� */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SD_SPI_CS_PORT, &GPIO_InitStructure);
	
  /* ����SPI�� SCK����*/
  GPIO_InitStructure.GPIO_Pin = SD_SPI_SCK_PIN;
  GPIO_Init(SD_SPI_SCK_PORT, &GPIO_InitStructure);



  /* ����SPI�� MOSI����*/
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MOSI_PIN;
  GPIO_Init(SD_SPI_MOSI_PORT, &GPIO_InitStructure);

  /* ����SPI�� MISO����*/
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SD_SPI_MISO_PORT, &GPIO_InitStructure);
  /* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
  SPI_SD_CS_1;

 
	
}



























































