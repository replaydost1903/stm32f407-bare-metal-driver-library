#ifndef MAIN_H_
#define MAIN_H_

/*
 * 	Standart Library's
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/*
 *  Example usage 	:
 *  	READ_BIT(RCC->CR,RCC_CR_HSIRDY_Pos)
 */
#define READ_BIT(REG,BIT)							((((REG) >> (BIT)) & (0x1U)))
#define SET_BIT(REG,BIT)							((REG = ((((0x1U << (BIT)) | REG)))))
#define CLEAR_BIT(REG,BIT)							((REG = (((~(0x1U << (BIT))) & REG))))

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
