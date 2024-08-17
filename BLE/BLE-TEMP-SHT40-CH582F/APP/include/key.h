/*
 * key.h
 *
 *  Created on: 2024Äê7ÔÂ22ÈÕ
 *      Author: JQW
 */

#ifndef INCLUDE_KEY_H_
#define INCLUDE_KEY_H_

#include "CH58x_common.h"

#define BOOT_KEY  GPIO_Pin_4

void boot_key_init();
uint8_t boot_key_dectect();
void boot_key_run();


#endif /* INCLUDE_KEY_H_ */
