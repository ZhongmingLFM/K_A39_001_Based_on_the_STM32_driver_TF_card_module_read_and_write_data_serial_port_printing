#ifndef __TF_CARD_SPI_H
#define __TF_CARD_SPI_H
										  	  
#include "stm32f10x.h"


//CS(NSS)引脚 片选选普通GPIO即可

#define      SD_SPI_ALL_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      SD_SPI_ALL_CLK                 RCC_APB2Periph_GPIOB 


#define      SD_SPI_CS_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define      SD_SPI_CS_CLK                  RCC_APB2Periph_GPIOB    
#define      SD_SPI_CS_PORT                 GPIOB
#define      SD_SPI_CS_PIN                  GPIO_Pin_6

//SCK引脚
#define      SD_SPI_SCK_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      SD_SPI_SCK_CLK                 RCC_APB2Periph_GPIOB   
#define      SD_SPI_SCK_PORT                GPIOB   
#define      SD_SPI_SCK_PIN                 GPIO_Pin_7
//MISO引脚
#define      SD_SPI_MISO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      SD_SPI_MISO_CLK                RCC_APB2Periph_GPIOB    
#define      SD_SPI_MISO_PORT               GPIOB 
#define      SD_SPI_MISO_PIN                GPIO_Pin_8
//MOSI引脚
#define      SD_SPI_MOSI_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      SD_SPI_MOSI_CLK                RCC_APB2Periph_GPIOB    
#define      SD_SPI_MOSI_PORT               GPIOB 
#define      SD_SPI_MOSI_PIN                GPIO_Pin_9

#define  		SPI_SD_CS_0    						GPIO_ResetBits( SD_SPI_CS_PORT, SD_SPI_CS_PIN )
#define  		SPI_SD_CS_1    						GPIO_SetBits( SD_SPI_CS_PORT, SD_SPI_CS_PIN )

#define  		SD_SPI_MOSI_0    					GPIO_ResetBits( SD_SPI_MOSI_PORT, SD_SPI_MOSI_PIN )
#define  		SD_SPI_MOSI_1    					GPIO_SetBits( SD_SPI_MOSI_PORT, SD_SPI_MOSI_PIN )

#define  		SD_SPI_SCK_0     					GPIO_ResetBits( SD_SPI_SCK_PORT, SD_SPI_SCK_PIN )
#define  		SD_SPI_SCK_1    					GPIO_SetBits( SD_SPI_SCK_PORT, SD_SPI_SCK_PIN )

#define  		READ_SD_SPI_MISO    			GPIO_ReadInputDataBit( SD_SPI_MISO_PORT, SD_SPI_MISO_PIN )
void SPI_SD_Init(void);

#endif






































