#ifndef BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_RCC_H_
#define BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_RCC_H_

#include "main.h"


/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Güç tasarruf modunda işlemcinin güç tüketimi ve çektiği akımlar karşılaştırılacak
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


#define __SYS_CLK_HSI_SELECT__()						do														\
														{														\
															RCC->CFGR |= RCC_CFGR_SW_HSI;						\
														}while((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS);

#define __SYS_CLK_HSE_SELECT__()						do														\
														{														\
															RCC->CFGR |= RCC_CFGR_SW_HSE;						\
														}while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_0);

#define __SYS_CLK_PLL_SELECT__()						do														\
														{														\
															RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);	\
														}while(!(RCC->CFGR & RCC_CFGR_SWS_1));

#define __HSI_ENABLE__()								do														\
														{														\
															RCC->CR |= RCC_CR_HSION;							\
														}while((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);


#define __HSE_ENABLE__()								do														\
														{														\
															RCC->CR |= RCC_CR_HSEON;							\
														}while((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);

#define __PLL_ENABLE__()								do														\
														{														\
															RCC->CR |= RCC_CR_PLLON;							\
														}while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);


#define __HSI_DISABLE()									((RCC->CR 	&= ~(0x1U << RCC_CR_HSION_Pos)))

#define __HSE_DISABLE()									((RCC->CR 	&= ~(0x1U << RCC_CR_HSEON_Pos)))

#define __LSI_DISABLE()									((RCC->CSR 	&= ~(0x1U << RCC_CSR_LSION_Pos)))

#define __LSE_DISABLE()									((RCC->BDCR &= ~(0x1U << RCC_BDCR_LSEON_Pos)))

#define __PLL_DISABLE()									((RCC->CR 	&= ~(0x1U << RCC_CR_PLLON_Pos)))

/*
 * 	@ref RCC_SYSCLK_SRC
 */

#define SYSCLK_SRC_HSI						(RCC_CFGR_SW_HSI)
#define SYSCLK_SRC_HSE						(RCC_CFGR_SW_HSE)
#define SYSCLK_SRC_PLL						(RCC_CFGR_SW_PLL)

/*
 * @ref RCC_HSE_SRC
 */

#define HSE_NOT_BYPASS						(0x00000000U)
#define HSE_BYPASS							(RCC_CR_HSEBYP)

/*
 * @ref RCC_HSE_CSSON
 */
#define HSE_CSSON_ON						(RCC_CR_CSSON)
#define HSE_CSSON_OFF						(0x00000000U)

/*
 * @ref RCC_PLL_SRC
 */
#define PLL_SRC_HSI							(RCC_PLLCFGR_PLLSRC_HSI)
#define PLL_SRC_HSE							(RCC_PLLCFGR_PLLSRC_HSE)

/*
 * @ref RCC_PLLQ_DIV
 */
#define PLL_Q_DIV2							(0x2U << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV3							(0x3U << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV4							(0x4U << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV5							(0x5U << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV6							(0x6U << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV7							(0x7U << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV8							(0x8U << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV9							(0x9U << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV10							(0xAU << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV11							(0xBU << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV12							(0xCU << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV13							(0xDU << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV14							(0xEU << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV15							(0xFU << RCC_PLLCFGR_PLLQ_Pos)

/*
 * @ref RCC_PLLP_DIV
 */
#define PLL_P_DIV2							(0x00000000U)
#define PLL_P_DIV4							(0x01U << RCC_PLLCFGR_PLLP_Pos)
#define PLL_P_DIV6							(0x02U << RCC_PLLCFGR_PLLP_Pos)
#define PLL_P_DIV8							(0x03U << RCC_PLLCFGR_PLLP_Pos)

/*
 * @ref RCC_PLL_JITTER
 */
#define PLL_JITTER_OFF						(0U)
#define PLL_JITTER_ON						(1U)


/*
 * @ref RCC_MCO1_SRC
 */
#define MCO1_SRC_HSI						(0x0000000U)
#define MCO1_SRC_LSE						(0x1U << RCC_CFGR_MCO1_Pos)
#define MCO1_SRC_HSE						(0x2U << RCC_CFGR_MCO1_Pos)
#define MCO1_SRC_PLL						(0x3U << RCC_CFGR_MCO1_Pos)

/*
 * @ref RCC_MCO1_DIV
 */
#define MCO1_DIV_2							(0x4U << RCC_CFGR_MCO1PRE_Pos)
#define MCO1_DIV_3							(0x5U << RCC_CFGR_MCO1PRE_Pos)
#define MCO1_DIV_4							(0x6U << RCC_CFGR_MCO1PRE_Pos)
#define MCO1_DIV_5							(0x7U << RCC_CFGR_MCO1PRE_Pos)

/*
 * @ref RCC_MCO2_SRC
 */
#define MCO2_SRC_SYSCLK						(0x0000000U)
#define MCO2_SRC_PLLI2S						(0x1U << RCC_CFGR_MCO2_Pos)
#define MCO2_SRC_HSE						(0x2U << RCC_CFGR_MCO2_Pos)
#define MCO2_SRC_PLL						(0x3U << RCC_CFGR_MCO2_Pos)

/*
 * @ref RCC_MCO2_DIV
 */
#define MCO2_DIV_2							(0x4U << RCC_CFGR_MCO2PRE_Pos)
#define MCO2_DIV_3							(0x5U << RCC_CFGR_MCO2PRE_Pos)
#define MCO2_DIV_4							(0x6U << RCC_CFGR_MCO2PRE_Pos)
#define MCO2_DIV_5							(0x7U << RCC_CFGR_MCO2PRE_Pos)

/*
 * @ref RCC_MCOx
 */
#define RCC_MCO1							(1U)
#define RCC_MCO2							(2U)

/*
 * @ref RCC_POWER_SAVE
 */
#define RCC_HSI_POWER_SAVE					(1U)
#define RCC_HSE_POWER_SAVE					(2U)
#define RCC_LSI_POWER_SAVE					(4U)
#define RCC_LSE_POWER_SAVE					(8U)
#define RCC_PLL_POWER_SAVE					(16U)














/*
 * @ahb_prescaler_value
 */
#define AHB_PRESCALER_1					(0U)
#define AHB_PRESCALER_2					(8U)
#define AHB_PRESCALER_4					(9U)
#define AHB_PRESCALER_8					(10U)
#define AHB_PRESCALER_16				(11U)
#define AHB_PRESCALER_64				(12U)
#define AHB_PRESCALER_128				(13U)
#define AHB_PRESCALER_256				(14U)
#define AHB_PRESCALER_512				(15U)

/*
 * @apb1_prescaler_value
 */
#define APB1_PRESCALER_1				(0U)
#define APB1_PRESCALER_2				(4U)
#define APB1_PRESCALER_4				(5U)
#define APB1_PRESCALER_8				(6U)
#define APB1_PRESCALER_16				(7U)

/*
 * @apb2_prescaler_value
 */
#define APB1_PRESCALER_1				(0U)
#define APB2_PRESCALER_2				(4U)
#define APB2_PRESCALER_4				(5U)
#define APB2_PRESCALER_8				(6U)
#define APB2_PRESCALER_16				(7U)










///*
// * @system_clock_prescaler
// */
//typedef struct
//{
//	uint8_t ahb_pres;				/*<!		@ahb_prescaler_value		!>*/
//
//	uint8_t apb1_pres;				/*<!		@apb2_prescaler_value		!>*/
//
//	uint8_t apb2_pres;				/*<!		@apb2_prescaler_value		!>*/
//
//}Sys_PresValue;

/*
 *  @RCC_OSC_FLAG
 */
typedef struct
{
	uint8_t LSI_RDY				:1;

	uint8_t LSE_RDY				:1;

	uint8_t HSI_RDY				:1;

	uint8_t HSE_RDY				:1;

	uint8_t PLL_RDY				:1;

	uint8_t PLLI2S_RDY			:1;

	uint8_t RESERVED			:2;

}RCC_OscStatusTypeDef;

/**
  * @brief  Initializes the CPU, AHB and APB busses clocks according to the specified
  *         parameters in the RCC_ClkInitStruct.
  * @param  RCC_ClkInitStruct pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC peripheral.
  * @param  FLatency FLASH Latency, this parameter depend on device selected
  *
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency
  *         and updated by HAL_RCC_GetHCLKFreq() function called within this function
  *
  * @retval None
  */
typedef struct
{
	uint32_t Source;						/*!< This determines the source of the PLL clock.
	                                      	  	  	  This parameter can be a value of @ref RCC_PLL_SRC.      						*/

	uint32_t Q;								/*!< This determines the division factor Q of the PLL clock.
	                                      	  	  	  This parameter can be a value of @ref RCC_PLLQ_DIV.      						*/

	uint32_t P;								/*!< This determines the division factor P of the PLL clock.
	                                      	  	  	  This parameter can be a value of @ref RCC_PLLP_DIV.	      					*/

	uint32_t N;								/*!< This determines the division factor N of the PLL clock.
	                                      	  	  	  This parameter must be equal to or between 50 or 432. 		     			*/

	uint32_t M;								/*!< This determines the division factor M of the PLL clock.
	                                      	  	  	  This parameter must be equal to or between 2 or 63.    						*/

	uint32_t HSEFreq;						/*!< Entering the correct HSE frequency enables PLL_JITTER to be activated.
	                                      	  	  	  The HSE clock frequency must be equal to or between 4 or 26.       			*/

	uint32_t Jitter;						/*!< This allows to select the state of the pll jitter. When PLL Jitter is activated,
												 PLL_M value -> PLL input clock frequency (HSE or HSI frequency) / 2 is changed.
	 	 	 	 	 	 	 	 	 	 	 	 The user should be aware of this when calculating the system clock.
	                                      	  	  	  This parameter can be a value of @ref RCC_PLL_JITTER.     		    		*/

}PLL_InitTypeDef;


/**
  * @brief  Initializes the CPU, AHB and APB busses clocks according to the specified
  *         parameters in the RCC_ClkInitStruct.
  * @param  RCC_ClkInitStruct pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC peripheral.
  * @param  FLatency FLASH Latency, this parameter depend on device selected
  *
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency
  *         and updated by HAL_RCC_GetHCLKFreq() function called within this function
  *
  * @retval None
  */
typedef struct
{
	uint32_t SYSCLKSource;					/*!< This determines the source of the system clock.
	                                      	  	  	  	  This parameter can be a value of @ref RCC_SYSCLK_SRC      */

	uint32_t HSEBypass;						/*!< The new state of the HSE.
	                                      	  	  	  	  This parameter can be a value of @ref RCC_HSE_SRC      	*/

	uint32_t HSECSSONState;					/*!< This activates the clock security system for the HSE oscillator
	                                      	  	  	  	  This parameter can be a value of @ref RCC_HSE_CSSON     	*/

	uint32_t HSICalibrationValue;			/*!< This allows the calibration value to be written to the HSITRIM[4:0] register, this value must be equal to 0 or 32.
	                                      	  	  	  	  By default HSITRIM[4:0] = 0x10. See AN5067 for detailed calibration information 	*/

	PLL_InitTypeDef PLL;					/*!< If PLL is selected as the system clock, PLL setting is made with this expression
		                                      	  	  	  This parameter can be a value of @ref RCC_PLL_CONFIG      */

	uint32_t PowerSavingMode;				/*!< This parameter allows the user to disable the clocks that are not used in the system to save power.
													If the user disables the clocks used to save power, the Hard_Fault handler interrupts.
	 	 	 	 	 	 	 	 						  This parameter can be a value of @ref RCC_POWER_SAVE */

	RCC_OscStatusTypeDef OscState;			/*!< This parameter is contained in the RCC_Handler function and provides information to the user about
	 	 	 	 	 	 	 	 	 	 	 	 	 whether the oscillators are initialised properly.
	 	 	 	 	 	 	 	 	 	 	 	 	 Caution : Since HSI is already internally initialised in the system, it is not in the READY state in the interrupt flag.
	 	 	 	 	 	 	 	 	 	 	 	  	 If the user changes the system oscillator source, switches off the HSI and restarts it, it returns to the READY state.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 This parameter can be a value of @ref RCC_OSC_FLAG      */

}RCC_SysClkInitTypeDef;




void System_Init(void);
StatusFlagTypeDef RCC_SysClkInit(RCC_SysClkInitTypeDef*);
void RCC_Handler(RCC_SysClkInitTypeDef*);
void MCO_Output(uint32_t,uint32_t,uint32_t);
//StatusFlagTypeDef System_Clock_Init(RCC_Handle_TypeDef *);





#endif /* BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_RCC_H_ */
