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

#include "TF_Card_SPI.h"
#include "delay.h"


/**
  * @brief  SPI_SD引脚初始化
  * @param  无
  * @retval 无
  */
void SPI_SD_Init(void)
{
 
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能SPI引脚相关的时钟 */
 	SD_SPI_ALL_APBxClock_FUN ( SD_SPI_ALL_CLK, ENABLE );
	
  /* 配置SPI的 CS引脚，普通IO即可 */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SD_SPI_CS_PORT, &GPIO_InitStructure);
	
  /* 配置SPI的 SCK引脚*/
  GPIO_InitStructure.GPIO_Pin = SD_SPI_SCK_PIN;
  GPIO_Init(SD_SPI_SCK_PORT, &GPIO_InitStructure);



  /* 配置SPI的 MOSI引脚*/
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MOSI_PIN;
  GPIO_Init(SD_SPI_MOSI_PORT, &GPIO_InitStructure);

  /* 配置SPI的 MISO引脚*/
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SD_SPI_MISO_PORT, &GPIO_InitStructure);
  /* 停止信号 FLASH: CS引脚高电平*/
  SPI_SD_CS_1;

 
	
}



























































