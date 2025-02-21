#ifndef MAIN_H_
#define MAIN_H_

/*
 *  Common Library Data Type
 */
typedef enum
{
	ERROR,
	OK,
	BUSY

}StatusFlagTypeDef;


/*
 *  Microcontroller Library
 */
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include "stm32f407xx.h"

/*
 *  Core Library
 */
#include "cmsis_version.h"
#include "cmsis_compiler.h"
#include "cmsis_gcc.h"
#include "core_cm4.h"

/*
 *  My Library
 */
#include "stm32f407_rcc.h"
#include "stm32f407_gpio.h"

#endif /* MAIN_H_ */
