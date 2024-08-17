#ifndef SHT40_H
#define SHT40_H

#include "CONFIG.h"
#include "CH58x_common.h"

#define PIN_SCL GPIO_Pin_13
#define PIN_SDA GPIO_Pin_12
   	   		   

// 函数声明
void readSHT40(float *temperature, float *humidity);
void readSHT40_step1();
void readSHT40_step2(float *temperature, float *humidity);

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口
void IIC_Deinit();
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

void SDA_IN();
void SDA_OUT();
void SCL_HIGH();
void SCL_LOW();
void SDA_HIGH();
void SDA_LOW();
u8 READ_SDA();


#endif
















