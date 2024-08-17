/*
 * boot.c
 *
 *  Created on: 2024��7��22��
 *      Author: JQW
 *      ��ת��boot
 */

#include "boot.h"


/*******************************************************************************
 * Function Name  : EnterCodeUpgrade
 * Description    : ����BOOT����׼����������
 * Input          : None
 * Return         : None
 *******************************************************************************/
__HIGH_CODE
void APPJumpBoot(void)   //�˶δ������������RAM��
{
    while(FLASH_ROM_ERASE(0,EEPROM_BLOCK_SIZE))
    {
        ;   //ROM ��4K1����λ����0��ַ��ʼ
    }
    FLASH_ROM_SW_RESET();
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    SAFEOPERATE
    ;
    R16_INT32K_TUNE = 0xFFFF;
    R8_RST_WDOG_CTRL |= RB_SOFTWARE_RESET;
    R8_SAFE_ACCESS_SIG = 0;   //�����ִ�и�λ����λ����Ϊ�ϵ縴λ
    while(1);
    //Ӫ���Ƭ����������ʱ�ͻ�ͣ��BOOT������д����ʱʱ��10s
}






