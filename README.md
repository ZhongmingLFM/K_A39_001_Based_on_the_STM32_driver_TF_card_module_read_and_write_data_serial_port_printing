这里有中英文说明 前部分为中文说明 后半部分为英文说明 请悉知

Here are Chinese and English description The previous part of the Chinese description is the English description in English, please know how to know

注：文档手册在工程文件的DOC文件夹内

Note: The document manual is in the DOC folder of the engineering file

如果发现程序有BUG 或者有可以改进的地方 欢迎联系我们

If you find that the program has bugs or can be improved, please contact us

- [所有资源导航](#所有资源导航)
- [资源说明](#资源说明)
- [基本参数](#基本参数)
  * [参数](#参数)
  * [引脚说明](#引脚说明)
- [驱动说明](#驱动说明)
  * [时序](#时序)
  * [对应程序](#对应程序)
- [部分代码说明](#部分代码说明)
  * [接线引脚定义](#接线引脚定义)
    + [STM32F103C8T6+TF卡模块](#STM32F103C8T6+TF卡模块)
- [基础知识学习与相关资料下载](#基础知识学习与相关资料下载)
- [视频效果展示与程序资料获取](#视频效果展示与程序资料获取)
- [注意事项](#注意事项)
- [接线说明](#接线说明)
  * [STM32F103C8T6](#stm32f103c8t6)
- [All resource navigation](#all-resource-navigation)
- [Resource description](#resource-description)
- [basic parameters](#basic-parameters)
  * [parameter](#parameter)
  * [pin explanation](#pin-explanation)
- [Drive Instructions](#drive-instructions)
  * [timing](#timing)
  * [corresponding program:](#corresponding-program-)
- [Partial code description](#partial-code-description)
  * [Wiring pin definition](#wiring-pin-definition)
    + [STM32F103C8T6+TF card module](#stm32f103c8t6-tf-card-module)
- [Basic knowledge learning and related information download](#basic-knowledge-learning-and-related-information-download)
- [6. Video effect display and program data acquisition](#6-video-effect-display-and-program-data-acquisition)
- [Notice](#notice)
- [Wiring description](#wiring-description)
  * [STM32F103C8T6](#stm32f103c8t6-1)

@[TOC](K_A39_001  基于STM32驱动TF卡模块读写数据 串口打印)


![请添加图片描述](https://img-blog.csdnimg.cn/fc013e7828434e9db93c9ddb35d2d15c.jpeg)


#  所有资源导航
[**其他资料目录**](https://blog.csdn.net/lfmnlxx79691557/article/details/128323460?spm=1001.2014.3001.5501)     	直戳跳转
# 资源说明
| 单片机型号 | 测试条件|模块名称 |代码功能 |
|:--------:| -------------:| -------------:|-------------:|
| STC89C52RC | 晶振11.0592M	 |TF卡模块|STC89C52RC驱动TF卡模块  串口显示 |
| STM32F103C8T6 | 晶振8M/系统时钟72M |TF卡模块 |STM32F103C8T6驱动TF卡模块 串口显示 |


# 基本参数
## 参数

![请添加图片描述](https://img-blog.csdnimg.cn/87ce16f04b30443e8a3d66a736e9ed2f.png)



## 引脚说明

TF卡模块	| 引脚说明
-------- | -----
VCC	| 正极 5V供电
GND	| GND 电源负极
MISO	| SPI总线 主机输入从机输出引脚
MOSI	| SPI总线 主机输出从机输入引脚
SCK	| SPI总线  时钟引脚
CS	| 片选信号脚


# 驱动说明

## 时序

SPI时序
## 对应程序

```javascript
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
```

# 部分代码说明
## 接线引脚定义
需要自定义引脚可在此处更改，STM32要自定义引脚的话也要注意引脚时钟使能的更改

### STM32F103C8T6+TF卡模块
```javascript
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
```


# 基础知识学习与相关资料下载
1、[STC89C52RC程序下载](https://blog.csdn.net/lfmnlxx79691557/article/details/127114912?spm=1001.2014.3001.5501)			直戳跳转

2、STM32F103C8T6程序下载		
(1)[串口下载](https://blog.csdn.net/lfmnlxx79691557/article/details/122862183?spm=1001.2014.3001.5501)                直戳跳转 
(2)[ST-LINK下载](https://blog.csdn.net/lfmnlxx79691557/article/details/127140554?spm=1001.2014.3001.5501)             直戳跳转
(3)[J-LINK下载](https://blog.csdn.net/lfmnlxx79691557/article/details/127140823?spm=1001.2014.3001.5501)              直戳跳转
(4)[DAP-LINK下载](https://blog.csdn.net/lfmnlxx79691557/article/details/127141244?spm=1001.2014.3001.5501)            直戳跳转

3、[OLED0.96程序说明](https://blog.csdn.net/lfmnlxx79691557/article/details/127208887?spm=1001.2014.3001.5501)			直戳跳转

4、串口助手下载与使用
(1)[安信可调试助手使用](https://blog.csdn.net/lfmnlxx79691557/article/details/128335660?spm=1001.2014.3001.5502) 						直戳跳转
(2) [sscom33串口调试助手使用](https://blog.csdn.net/lfmnlxx79691557/article/details/128335808?spm=1001.2014.3001.5502)			直戳跳转
(3)[STC-ISP串口调试助手使用](https://blog.csdn.net/lfmnlxx79691557/article/details/128335896?spm=1001.2014.3001.5502)			    直戳跳转

# 视频效果展示与程序资料获取
[视频连接](https://www.bilibili.com/video/BV1gz4y1a7hb/?vd_source=7933b82b52c9cbf1c7966eb9f0dbd29f)  	直戳跳转
资料获取            可直接下载仓库资料

# 注意事项
1、VCC GND请勿接反，接反易烧
2、OLED显示异常时，排除接线接触不良

# 接线说明

## STM32F103C8T6 
```javascript
  
/************************************************************************************
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
```

@[TOC](K_A39_001 Based on the STM32 driver TF card module read and write data serial port printing)

![Please add a picture description](https://img-blog.csdnimg.cn/fc013e7828434e9db93c9ddb35d2d15c.jpeg)
# All resource navigation
[ Other data catalog ](https://blog.csdn.net/lfmnlxx79691557/article/details/128323460?spm=1001.2014.3001.5501)  Direct link to jump
# Resource description
| Single -chip Machine Model | Test Conditions | Module Name | Code Function |
|:--------:| -------------:| -------------:|-------------:| 
| STC89C52RC | Crystal Episode 11.0592M | TF Card Motors | STC89C52RC Drives TF Card Module serial port display |
| STM32F103C8T6 | Crystal Vibration 8M/System Clock 72M | TF card module | STM32F103C8T6 Drive TF card module serial port display |


# basic parameters
## parameter
![Please add picture description](https://img-blog.csdnimg.cn/87ce16f04b30443e8a3d66a736e9ed2f.png)


## pin explanation

TF card module | pin explanation
-------- | -------
Vcc | Power 5V power supply
GND | GND power supply negative electrode
MISO | SPI bus host input from the machine output pin
MOSI | SPI bus host output from the machine input pin from the machine
SCK | SPI bus clock pin
CS | Selection signal pin


#  Drive Instructions

## timing

SPI timing
## corresponding program:

```javascript
/**
   * @Brief SPI_SD pin initialization
   * @param None
   * @Retval none
   */
void SPI_SD_Init (void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
	/*Make clocks related to SPI pin */
	SD_SPI_ALL_APBxClock_FUN ( SD_SPI_ALL_CLK, ENABLE );

	/*Configure the SPI CS pin, ordinary IO can*/
	GPIO_InitStructure.GPIO_Pin = SD_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SD_SPI_CS_PORT, &GPIO_InitStructure);

	/*Configure the SPI SCK pin*/
	GPIO_InitStructure.GPIO_Pin = SD_SPI_SCK_PIN;
	GPIO_Init(SD_SPI_SCK_PORT, &GPIO_InitStructure);

	/* Configure the SPI MOSI pin*/
	Gpio_initStructure.gpio_pin = sd_spi_mosi_pin;
	Gpio_init (SD_SPI_MOSI_PORT, & GPIO_INITSTRUCTURE);

	/*Configure SPI Miso pin*/
	GPIO_InitStructure.GPIO_Pin = SD_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SD_SPI_MISO_PORT, &GPIO_InitStructure);
	/* Stop signal Flash: CS pin high level*/
	SPI_SD_CS_1;

}


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

// *********************************************
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
// *********************************************
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


// *********************************************
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
```

# Partial code description
## Wiring pin definition
You need to customize the pin can be changed here. If STM32 needs to customize the pin, you should also pay attention

### STM32F103C8T6+TF card module
```javascript
#define      SD_SPI_ALL_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      SD_SPI_ALL_CLK                 RCC_APB2Periph_GPIOB 


#define      SD_SPI_CS_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define      SD_SPI_CS_CLK                  RCC_APB2Periph_GPIOB    
#define      SD_SPI_CS_PORT                 GPIOB
#define      SD_SPI_CS_PIN                  GPIO_Pin_6

//SCK Pin
#define      SD_SPI_SCK_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      SD_SPI_SCK_CLK                 RCC_APB2Periph_GPIOB   
#define      SD_SPI_SCK_PORT                GPIOB   
#define      SD_SPI_SCK_PIN                 GPIO_Pin_7
//MISO Pin
#define      SD_SPI_MISO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      SD_SPI_MISO_CLK                RCC_APB2Periph_GPIOB    
#define      SD_SPI_MISO_PORT               GPIOB 
#define      SD_SPI_MISO_PIN                GPIO_Pin_8
//MOSI Pin
#define      SD_SPI_MOSI_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      SD_SPI_MOSI_CLK                RCC_APB2Periph_GPIOB    
#define      SD_SPI_MOSI_PORT               GPIOB 
#define      SD_SPI_MOSI_PIN                GPIO_Pin_9
```

# Basic knowledge learning and related information download

1. [STC89C52RC program download](https://blog.csdn.net/lfmnlxx79691557/article/details/127114912?spm=1001.2014.3001.5501)  Direct link to jump

2. STM32F103C8T6 programload		

	(1) [serial port download](https://blog.csdn.net/lfmnlxx79691557/article/details/122862183?spm=1001.2014.3001.5501) Direct link to jump

	(2) [ST-LINK Download](https://blog.csdn.net/lfmnlxx79691557/article/details/127140554?spm=1001.2014.3001.5501)  Direct link to jump

	(3) [j-link download](https://blog.csdn.net/lfmnlxx79691557/article/details/127140823?1001.2014.3001.5501) Direct link to jump

	(4) [dap-link download](https://blog.csdn.net/lfmnlxx79691557/article/details/127141244?spm=1001.2014.3001.5501) Direct link to jump

3. [OLED0.96 Program Instructions](https://blog.csdn.net/lfmnlxx79691557/article/details/127208887?spm=1001.2014.3001.5501)  Direct link to jump

4. Download and use serial assistant

	(1) [Anxin's debugging assistant use](https://blog.csdn.net/lfmnlxx79691557/article/details/128335660?spm=1001.2014.3001.5502) Direct link to jump

	(2) [SSCOM33 serial debug assistant use](https://blog.csdn.net/lfmnlxx79691557/article/details/128335808?spm=1001.2014.3001.5502) Direct link to jump

	(3) [STC-ISP serial debugging assistant use](https://blog.csdn.net/lfmnlxx79691557/article/details/128335896?spm=1001.2014.3001.5502) Direct link to jump

# 6. Video effect display and program data acquisition

[Video Connection](https://www.bilibili.com/video/BV1gz4y1a7hb/?vd_source=7933b82b52c9cbf1c7966eb9f0dbd29f)  Direct link to jump

Data acquisition (can download the warehouse directly)



# Notice

1. Please do not recalculate VCC GND.

2. When the OLED display is abnormal, exclude poor wiring contact

# Wiring description

## STM32F103C8T6
```javascript
  
/*************************************************** *****************************************
// Function description: TF card module test program
// Test conditions: STM32F103C8T6 Crystal 8M System Clock 72M
wiring
TF Module-------------------------------STM32F103C8T6
VCC------------------------------------3.3/5V
GND------------------------------------GND
CS-------------------------------------PB6 //
SCK------------------------------------PB7 //	
MISO-----------------------------------PB8 //	
MOSI-----------------------------------PB9 //	


************************************************* ************************************************
```
