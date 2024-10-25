/*
 * 		Written By Kubilay Közleme
 */

#ifndef BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_RCC_H_
#define BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_RCC_H_

#include "main.h"

/*
 * @ahb_prescaler_value
 */
#define AHB_PRESCALER_1			(0U)
#define AHB_PRESCALER_2			(8U)
#define AHB_PRESCALER_4			(9U)
#define AHB_PRESCALER_8			(10U)
#define AHB_PRESCALER_16		(11U)
#define AHB_PRESCALER_64		(12U)
#define AHB_PRESCALER_128		(13U)
#define AHB_PRESCALER_256		(14U)
#define AHB_PRESCALER_512		(15U)

/*
 * @apb1_prescaler_value
 */
#define APB1_PRESCALER_1		(0U)
#define APB1_PRESCALER_2		(4U)
#define APB1_PRESCALER_4		(5U)
#define APB1_PRESCALER_8		(6U)
#define APB1_PRESCALER_16		(7U)

/*
 * @apb2_prescaler_value
 */
#define APB1_PRESCALER_1		(0U)
#define APB2_PRESCALER_2		(4U)
#define APB2_PRESCALER_4		(5U)
#define APB2_PRESCALER_8		(6U)
#define APB2_PRESCALER_16		(7U)

/*
 *  @mco_mode
 */
#define MCO_MODE_1_ENABLE		(0U)
#define MCO_MODE_2_ENABLE		(1U)
#define MCO_ALL_MODE_ENABLE		(2U)

/*
 *	@mco1_clk_src
 */
#define MCO1_CLK_SRC_HSI	(0U)
#define MCO1_CLK_SRC_LSE	(1U)
#define MCO1_CLK_SRC_HSE	(2U)
#define MCO1_CLK_SRC_PLL	(3U)

/*
 *	@mco2_clk_src
 */
#define MCO2_CLK_SRC_HSE	(0U)
#define MCO2_CLK_SRC_PLL	(1U)
#define MCO2_CLK_SRC_SYSCLK	(2U)
#define MCO2_CLK_SRC_PLLI2S	(3U)

/*
 *  @mco_prescaler
 */
#define MCO_PRE_1			(0U)
#define MCO_PRE_2			(4U)
#define MCO_PRE_3			(5U)
#define MCO_PRE_4			(6U)
#define MCO_PRE_5			(7U)

/*
 * @clock_source
 */
#define SYS_CLK_SRC_HSI		(0U)
#define SYS_CLK_SRC_HSE		(1U)
#define SYS_CLK_SRC_PLL		(2U)

/*
 * @hse_src
 */
#define HSE_NOT_BYPASS		(0U)
#define HSE_BYPASS			(1U)

/*
 * @hse_state
 */
#define HSE_OFF				(0U)
#define HSE_ON				(1U)

/*
 * @pll_src
 */
#define PLL_SRC_HSI			(0U)
#define PLL_SRC_HSE			(1U)

/*
 * @pll_jitter
 */
#define PLL_JIT_OFF			(0U)
#define PLL_JIT_ON			(1U)


/*
 *  @hsi_state
 */
#define HSI_OFF				(0U)
#define HSI_ON				(1U)

/*
 * @pll_P
 */
#define PLL_P_2				(2U)
#define PLL_P_4				(4U)
#define PLL_P_6				(6U)
#define PLL_P_8				(8U)

/*
 * @mco_output
 */
typedef struct
{
	uint8_t mco1;					/*<! 	@mco1_clk_src 			MCO1 Pin ->	PA8 !>*/
	uint8_t	mco2;					/*<! 	@mco2_clk_src 			MCO2 Pin ->	PC9 !>*/
	uint8_t mco1_pres;				/*<! 	@mco_prescaler							!>*/
	uint8_t mco2_pres;				/*<! 	@mco_prescaler							!>*/
	uint8_t mco_out_mode;			/*<! 	@mco_mode								!>*/


}MCO_ClkSrc;

/*
 *  @rcc_int_flag
 */
typedef struct
{
	uint8_t LSI_RDY:1;
	uint8_t LSE_RDY:1;
	uint8_t HSI_RDY:1;
	uint8_t HSE_RDY:1;
	uint8_t PLL_RDY:1;
	uint8_t PLLI2S_RDY:1;
	uint8_t RES:2;

}RCCFlagStatus;

/*
 * @system_clock_prescaler
 */
typedef struct
{
	uint8_t ahb_pres;				/*<!		@ahb_prescaler_value		!>*/
	uint8_t apb1_pres;				/*<!		@apb2_prescaler_value		!>*/
	uint8_t apb2_pres;				/*<!		@apb2_prescaler_value		!>*/

}Sys_PresValue;

/*
 * @pll_coefficient
 */
typedef struct
{
	uint32_t pll_Q;					/*<! PLL_Q coefficient is necessary for USB OTG FS, SDIO, RNG clock  						!>*/
	uint32_t pll_P;					/*<! 	@pll_P 					!>*/
	uint32_t pll_N;
	uint32_t pll_M;
	uint8_t pll_jitter;				/*<! In this case, the entered pll_M coefficient does not matter  VCO=2MHz	 @pll_jitter	!>*/
	uint8_t pll_freq;				/*<! 	External Crystal Oscillator Value [for bypass or not bypass] 						!>*/
	uint8_t pll_src;				/*<! 	@pll_src 					!>*/

}PLL_Config;

/*
 * RCC Peripheral Structure Definition
 */
typedef struct
{
	uint8_t sys_clk_src;			/*<! 	@clock_source 				!>*/
	uint8_t hse_src;				/*<! 	@hse_src 					!>*/
	uint8_t hsi_cal;				/*<! 	@hsi_cal 					!>*/
	uint8_t hsi_state;				/*<! 	@hsi_state 					!>*/
	uint8_t hse_state;				/*<! 	@hse_state 					!>*/
	PLL_Config pll;					/*<! 	@pll_coefficient 			!>*/
	Sys_PresValue prescaler;		/*<! 	@system_clock_prescaler		!>*/
	MCO_ClkSrc mco_out;				/*<! 	@mco_output		 			!>*/
	RCCFlagStatus rcc_flag;			/*<! 	@rcc_int_flag	 			!>*/

}RCC_Handle_TypeDef;


/* RCC -> Clock Section */
void System_Init(void);
LibStatusFlag System_Osc_Config(RCC_Handle_TypeDef *);
LibStatusFlag System_Clock_Config(RCC_Handle_TypeDef *);
void MCO_Output(RCC_Handle_TypeDef *);
void RCC_Handler(RCC_Handle_TypeDef *);




















/*
 *  Reset Section
 */

























#endif /* BARE_METAL_DRIVER_LIBRARY_INC_STM32F407_RCC_H_ */
