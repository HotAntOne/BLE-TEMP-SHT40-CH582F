/*
 * key.c
 *
 *  Created on: 2024��7��22��
 *      Author: JQW
 *      ���boot����������������ת���ض�������
 */

#include "key.h"
#include "boot.h"


void boot_key_init()
{
  GPIOB_ModeCfg(BOOT_KEY, GPIO_ModeIN_PU);
  GPIOB_ITModeCfg(BOOT_KEY, GPIO_ITMode_FallEdge);
  PFIC_EnableIRQ(GPIO_B_IRQn);
}

uint8_t boot_key_dectect()
{
  if(GPIOB_ReadPortPin(BOOT_KEY))
    return 1;
  else {
    return 0;
  }
}

void boot_key_run()
{
//  ��ȡ���Ų���ת��boot��
  if(boot_key_dectect())
  {
    APPJumpBoot();
  }
}

__INTERRUPT
__HIGH_CODE
void GPIOB_IRQHandler(void)
{
  if(GPIOB_ReadITFlagBit(BOOT_KEY))
  {
    GPIOB_ClearITFlagBit(BOOT_KEY);
    APPJumpBoot();

  }else {
    GPIOB_ClearITFlagBit(0xffff);
  }
}


