#ifndef BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_RCC_H_
#define BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_RCC_H_

#include "main.h"

#define __SYS_CLK_HSI_SELECT__							do														\
														{														\
															RCC->CFGR |= RCC_CFGR_SW_HSI;						\
														}while((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS);

#define __SYS_CLK_HSE_SELECT__							do														\
														{														\
															RCC->CFGR |= RCC_CFGR_SW_HSE;						\
														}while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_0);

#define __SYS_CLK_PLL_SELECT__							do														\
														{														\
															RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);	\
														}while(!(RCC->CFGR & RCC_CFGR_SWS_1));

#define __HSI_ENABLE__									do														\
														{														\
															RCC->CR |= RCC_CR_HSION;						\
														}while((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);

#define __HSE_ENABLE__									do														\
														{														\
															RCC->CR |= RCC_CR_HSEON;							\
														}while((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);

#define __PLL_ENABLE__									do														\
														{														\
															RCC->CR |= RCC_CR_PLLON;							\
														}while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);


///*
// *  Maximumu PLLP,PLLQ,PLLM,PLLN değerleri define şeklinde tanımlanacak
// */
//#define PLLQ_MAX			(123U)			// -> example !!!


/*
 * 	@ref RCC_SYSCLK_SRC
 */

#define SYSCLK_SRC_HSI						(RCC_CFGR_SW_HSI)
#define SYSCLK_SRC_HSE						(RCC_CFGR_SW_HSE)
#define SYSCLK_SRC_PLL						(RCC_CFGR_SW_PLL)

/*
 * @ref RCC_HSE_SRC
 */

#define HSE_NOT_BYPASS						(0U)
#define HSE_BYPASS							(RCC_CR_HSEBYP)

/*
 * @ref RCC_HSE_CSSON
 */
#define HSE_CSSON_ENABLE					(RCC_CR_CSSON)
#define HSE_CSSON_DISABLE					(~RCC_CR_CSSON)

/*
 * @ref RCC_PLL_SRC
 */
#define PLL_SRC_HSI							(RCC_PLLCFGR_PLLSRC_HSI)
#define PLL_SRC_HSE							(RCC_PLLCFGR_PLLSRC_HSE)

/*
 * @ref RCC_PLLQ_DIV
 */
#define PLL_Q_DIV2							(0x2 << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV3							(0x3 << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV4							(0x4 << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV5							(0x5 << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV6							(0x6 << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV7							(0x7 << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV8							(0x8 << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV9							(0x9 << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV10							(0xA << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV11							(0xB << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV12							(0xC << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV13							(0xD << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV14							(0xE << RCC_PLLCFGR_PLLQ_Pos)
#define PLL_Q_DIV15							(0xF << RCC_PLLCFGR_PLLQ_Pos)

/*
 * @ref RCC_PLLP_DIV
 */
#define PLL_P_DIV2							(0U)
#define PLL_P_DIV4							(0x01 << RCC_PLLCFGR_PLLP_Pos)
#define PLL_P_DIV6							(0x02 << RCC_PLLCFGR_PLLP_Pos)
#define PLL_P_DIV8							(0x03 << RCC_PLLCFGR_PLLP_Pos)

/*
 * @ref RCC_PLL_JITTER
 */
#define PLL_JITTER_ENABLE					(1U)
#define PLL_JITTER_DISABLE					(~PLL_JITTER_ENABLE)

















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

/*
 *  @mco_mode
 */
#define MCO_MODE_1_ENABLE				(0U)
#define MCO_MODE_2_ENABLE				(1U)
#define MCO_ALL_MODE_ENABLE				(2U)

/*
 *	@mco1_clk_src
 */
#define MCO1_CLK_SRC_HSI				(0U)
#define MCO1_CLK_SRC_LSE				(1U)
#define MCO1_CLK_SRC_HSE				(2U)
#define MCO1_CLK_SRC_PLL				(3U)

/*
 *	@mco2_clk_src
 */
#define MCO2_CLK_SRC_HSE				(0U)
#define MCO2_CLK_SRC_PLL				(1U)
#define MCO2_CLK_SRC_SYSCLK				(2U)
#define MCO2_CLK_SRC_PLLI2S				(3U)

/*
 *  @mco_prescaler
 */
#define MCO_PRE_1						(0U)
#define MCO_PRE_2						(4U)
#define MCO_PRE_3						(5U)
#define MCO_PRE_4						(6U)
#define MCO_PRE_5						(7U)

/*
 * @pll_jitter
 */
#define PLL_JIT_OFF						(0U)
#define PLL_JIT_ON						(1U)



///*
// * @mco_output
// */
//typedef struct
//{
//	uint8_t mco1;					/*<! 	@mco1_clk_src 			MCO1 Pin ->	PA8 !>*/
//	uint8_t	mco2;					/*<! 	@mco2_clk_src 			MCO2 Pin ->	PC9 !>*/
//	uint8_t mco1_pres;				/*<! 	@mco_prescaler							!>*/
//	uint8_t mco2_pres;				/*<! 	@mco_prescaler							!>*/
//	uint8_t mco_out_mode;			/*<! 	@mco_mode								!>*/
//
//
//}MCO_ClkSrc;

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
 *  @rcc_int_flag
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

}RCC_StatusTypeDef;

/*
 * @ref RCC_PLL_CONFIG
 */
typedef struct
{
	uint32_t Source;						/*!< This determines the source of the PLL.
	                                      	  	  	  This parameter can be a value of @ref RCC_PLL_SRC.      				*/

	uint32_t Q;								/*!< This determines the division factor Q of the PLL.
	                                      	  	  	  This parameter can be a value of @ref RCC_PLLQ_DIV.      				*/

	uint32_t P;								/*!< This determines the division factor P of the PLL.
	                                      	  	  	  This parameter can be a value of @ref RCC_PLLP_DIV.      				*/

	uint32_t N;								/*!< This determines the division factor N of the PLL.
	                                      	  	  	  This parameter can be a value of @ref RCC_PLLN_DIV.      				*/

	uint32_t M;								/*!< This determines the division factor M of the PLL.
	                                      	  	  	  This parameter may be equal to or between 50 or 432.    				*/

	uint32_t HSEFreq;						/*!< Entering the correct HSE frequency enables PLL_JITTER to be activated.
	                                      	  	  	  This parameter may be equal to or between 4 or 26.       				*/

	uint32_t Jitter;						/*!< This allows to select the state of the pll jitter.
	                                      	  	  	  This parameter can be a value of @ref RCC_PLL_JITTER.     			    */

}PLL_InitTypeDef;

/*
 * RCC System Clock Configuration
 */
typedef struct
{
	uint32_t SYSCLKSource;					/*!< This determines the source of the System clock.
	                                      	  	  	  	  This parameter can be a value of @ref RCC_SYSCLK_SRC      */

	uint32_t HSEBypass;						/*!< The new state of the HSE.
	                                      	  	  	  	  This parameter can be a value of @ref RCC_HSE_SRC      	*/

	uint32_t HSECSSONState;					/*!< This activates the Clock Security System for the HSE oscillator
	                                      	  	  	  	  This parameter can be a value of @ref RCC_HSE_CSSON     	*/

	uint32_t HSICalibrationValue;

	PLL_InitTypeDef PLL;					/*!< If PLL is selected as the system clock, PLL setting is made with this expression
		                                      	  	  	  This parameter can be a value of @ref RCC_PLL_CONFIG      */

	RCC_StatusTypeDef OscStateFlag;			/*!< The new state of the HSE.
	                                      	  	  	  	  	This parameter can be a value of @ref clock_source      */

	uint32_t PowerSavingMode;				/*!< The new state of the HSE.
	                                      	  	  	  Bu parametre enable olduğunda güç tasarufu yapmak için kullanılmayan saatler devre dışı bırakılır
	 eğer sistemde kullanılan saat varsa örneğin LSE , bu saat devre dışı bırakılmayacaksa yazılmamalıdır

	 	 	 Örnek kullanım	:
	 	 	 	 	 	 -> 		HSE_POWER_SAVE		->		Parametre
	 	 	 	 	 	 	 	 	HSI_POWER_SAVE		->		Parametre
	 	 	 	 	 	 	 	 	LSE_POWER_SAVE		->		Parametre
	 	 	 	 	 	 	 	 	LSI_POWER_SAVE		->		Parametre

	 	 	 	 	 	 	 	 	uint32_t PowerSavingMode = HSE_POWER_SAVE | HSI_POWER_SAVE;
	 	 	 	 	 	 	 	 		Yukardaki durumda HSE ve HSI kapatılır diğer saatlerin durumu korunur
	 	 */

}RCC_SysClkInitTypeDef;














/* RCC -> Clock Section */
void System_Init(void);
StatusFlagTypeDef RCC_SysClkInit(RCC_SysClkInitTypeDef*);
void RCC_Handler(RCC_SysClkInitTypeDef*);
//void MCO_Output(RCC_SysClkInitTypeDef*);
//StatusFlagTypeDef System_Clock_Init(RCC_Handle_TypeDef *);






















/*
 *  Reset Section
 */

























#endif /* BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_RCC_H_ */
