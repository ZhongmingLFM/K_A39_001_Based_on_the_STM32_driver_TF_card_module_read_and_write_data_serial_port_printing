#ifndef __TF_CARD_H
#define __TF_CARD_H
										  	  
#include "stm32f10x.h"
#include "TF_Card_SPI.h"


uint16_t SD_CMD_Write(uint16_t CMDIndex,uint16_t CMDArg,uint16_t ResType,uint16_t CSLowRSV);//ResType:Response Type, send 1 for R1; send 2 for R1b; send 3 for R2.
uint16_t SD_Reset_Card(void);
uint16_t  SD_Initiate_Card(void);//Polling the card after reset
uint16_t SD_Get_CardInfo(void);//Read CSD register
uint16_t SD_Overall_Initiation(void);
uint16_t SD_Get_CardID(void);//Read CID register
uint16_t Read_Single_Block(uint32_t  BlockAddress);
uint16_t Write_Single_Block(uint32_t  BlockAddress);

extern uint16_t  ReadBuffer[128];
extern uint16_t  WriteBuffer[128];
#endif






































