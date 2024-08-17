#ifndef SHT40_H
#define SHT40_H

#include "CONFIG.h"
#include "CH58x_common.h"

#define PIN_SCL GPIO_Pin_13
#define PIN_SDA GPIO_Pin_12
   	   		   

// ��������
void readSHT40(float *temperature, float *humidity);
void readSHT40_step1();
void readSHT40_step2(float *temperature, float *humidity);

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��
void IIC_Deinit();
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

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
















