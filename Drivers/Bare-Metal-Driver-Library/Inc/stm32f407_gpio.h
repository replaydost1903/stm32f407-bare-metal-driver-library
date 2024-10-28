#ifndef BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_GPIO_H_
#define BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_GPIO_H_

#include "main.h"


/*
 * @ref GPIO_pins_define
 */
#define GPIO_PIN_0				(0x00000001UL)
#define GPIO_PIN_1				(0x00000002UL)
#define GPIO_PIN_2				(0x00000004UL)
#define GPIO_PIN_3				(0x00000008UL)
#define GPIO_PIN_4				(0x00000010UL)
#define GPIO_PIN_5				(0x00000020UL)
#define GPIO_PIN_6				(0x00000040UL)
#define GPIO_PIN_7				(0x00000080UL)
#define GPIO_PIN_8				(0x00000100UL)
#define GPIO_PIN_9				(0x00000200UL)
#define GPIO_PIN_10				(0x00000400UL)
#define GPIO_PIN_11				(0x00000800UL)
#define GPIO_PIN_12				(0x00001000UL)
#define GPIO_PIN_13				(0x00002000UL)
#define GPIO_PIN_14				(0x00004000UL)
#define GPIO_PIN_15				(0x00008000UL)

#define GPIO_MODE_Pos                           0U
#define GPIO_MODE                               (0x3UL << GPIO_MODE_Pos)
#define MODE_INPUT                              (0x0UL << GPIO_MODE_Pos)
#define MODE_OUTPUT                             (0x1UL << GPIO_MODE_Pos)
#define MODE_AF                                 (0x2UL << GPIO_MODE_Pos)
#define MODE_ANALOG                             (0x3UL << GPIO_MODE_Pos)
#define OUTPUT_TYPE_Pos                         4U
#define OUTPUT_TYPE                             (0x1UL << OUTPUT_TYPE_Pos)
#define OUTPUT_PP                               (0x0UL << OUTPUT_TYPE_Pos)
#define OUTPUT_OD                               (0x1UL << OUTPUT_TYPE_Pos)
#define EXTI_MODE_Pos                           16U
#define EXTI_MODE                               (0x3UL << EXTI_MODE_Pos)
#define EXTI_IT                                 (0x1UL << EXTI_MODE_Pos)
#define EXTI_EVT                                (0x2UL << EXTI_MODE_Pos)
#define TRIGGER_MODE_Pos                         20U
#define TRIGGER_MODE                            (0x7UL << TRIGGER_MODE_Pos)
#define TRIGGER_RISING                          (0x1UL << TRIGGER_MODE_Pos)
#define TRIGGER_FALLING                         (0x2UL << TRIGGER_MODE_Pos)

/** @defgroup GPIO_mode_define GPIO mode define
  * @brief GPIO Configuration Mode
  *        Elements values convention: 0x00WX00YZ
  *           - W  : EXTI trigger detection on 3 bits
  *           - X  : EXTI mode (IT or Event) on 2 bits
  *           - Y  : Output type (Push Pull or Open Drain) on 1 bit
  *           - Z  : GPIO mode (Input, Output, Alternate or Analog) on 2 bits
  * @{
  */
#define  GPIO_MODE_INPUT                        MODE_INPUT                                                  /*!< Input Floating Mode                   */
#define  GPIO_MODE_OUTPUT_PP                    (MODE_OUTPUT | OUTPUT_PP)                                   /*!< Output Push Pull Mode                 */
#define  GPIO_MODE_OUTPUT_OD                    (MODE_OUTPUT | OUTPUT_OD)                                   /*!< Output Open Drain Mode                */
#define  GPIO_MODE_AF_PP                        (MODE_AF | OUTPUT_PP)                                       /*!< Alternate Function Push Pull Mode     */
#define  GPIO_MODE_AF_OD                        (MODE_AF | OUTPUT_OD)                                       /*!< Alternate Function Open Drain Mode    */

#define  GPIO_MODE_ANALOG                       MODE_ANALOG                                                 /*!< Analog Mode  */

#define  GPIO_MODE_IT_RISING                    (MODE_INPUT | EXTI_IT | TRIGGER_RISING)                     /*!< External Interrupt Mode with Rising edge trigger detection          */
#define  GPIO_MODE_IT_FALLING                   (MODE_INPUT | EXTI_IT | TRIGGER_FALLING)                    /*!< External Interrupt Mode with Falling edge trigger detection         */
#define  GPIO_MODE_IT_RISING_FALLING            (MODE_INPUT | EXTI_IT | TRIGGER_RISING | TRIGGER_FALLING)   /*!< External Interrupt Mode with Rising/Falling edge trigger detection  */

#define  GPIO_MODE_EVT_RISING                   (MODE_INPUT | EXTI_EVT | TRIGGER_RISING)                     /*!< External Event Mode with Rising edge trigger detection             */
#define  GPIO_MODE_EVT_FALLING                  (MODE_INPUT | EXTI_EVT | TRIGGER_FALLING)                    /*!< External Event Mode with Falling edge trigger detection            */
#define  GPIO_MODE_EVT_RISING_FALLING           (MODE_INPUT | EXTI_EVT | TRIGGER_RISING | TRIGGER_FALLING)   /*!< External Event Mode with Rising/Falling edge trigger detection     */

/*
 * @ref GPIO_speeds_define
 */
#define GPIO_SPEED_LOW					(0U)
#define GPIO_SPEED_MED					(1U)
#define GPIO_SPEED_HIGH					(2U)
#define GPIO_SPEED_VERY_HIGH			(3U)

/*
 * @ref GPIO_pull_define
 */
#define GPIO_NO_PULL					(0U)
#define GPIO_PULL_UP					(1U)
#define GPIO_PULL_DOWN					(2U)

/*
 * @ref GPIO_alternate_define
 */
//#define GPIO_AF0

/*
 * 	GPIO_init_type_def gpio pins configuration data type
 */
typedef struct
{
	uint32_t Pin;			/*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_pins_define */
	uint32_t Mode;			/*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_modes_define */
	uint32_t Speed;			/*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_speeds_define */
	uint32_t Pull;			/*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_pull_define */
	uint32_t Lock;			/*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_pins_define */
	uint32_t Alternate;		/*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_alternate_define */

}GPIOInitTypeDef;


void GPIO_Config(GPIO_TypeDef*,GPIOInitTypeDef*);





#endif /* BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_GPIO_H_ */
