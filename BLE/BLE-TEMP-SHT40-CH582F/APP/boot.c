/*
 * boot.c
 *
 *  Created on: 2024年7月22日
 *      Author: JQW
 *      跳转到boot
 */

#include "boot.h"


/*******************************************************************************
 * Function Name  : EnterCodeUpgrade
 * Description    : 跳入BOOT程序，准备代码升级
 * Input          : None
 * Return         : None
 *******************************************************************************/
__HIGH_CODE
void APPJumpBoot(void)   //此段代码必须运行在RAM中
{
    while(FLASH_ROM_ERASE(0,EEPROM_BLOCK_SIZE))
    {
        ;   //ROM 擦4K1个单位，擦0地址起始
    }
    FLASH_ROM_SW_RESET();
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    SAFEOPERATE
    ;
    R16_INT32K_TUNE = 0xFFFF;
    R8_RST_WDOG_CTRL |= RB_SOFTWARE_RESET;
    R8_SAFE_ACCESS_SIG = 0;   //进入后执行复位，复位类型为上电复位
    while(1);
    //营造空片的现象，启动时就会停在BOOT，等烧写，超时时间10s
}






