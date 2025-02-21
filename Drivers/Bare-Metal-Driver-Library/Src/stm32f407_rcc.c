#include "stm32f407_rcc.h"


/*
 * 	Caution This function is take only a RCC_SysClkInitTypeDef type variable
 */

#define __PLL_COEFF_CONTROL__(x)			(x->PLL.PLL_M >= 2) & (x->PLL.PLL_M <= 63)		   	& \
											(x->PLL.PLL_N <= 432) & (x->PLL.PLL_N >= 50) 		& \
											((x->PLL.PLL_P == 2) | (x->PLL.PLL_P == 4) | (x->PLL.PLL_P == 6) | (x->PLL.PLL_P == 8))

/*
 * HSI RC Oscillator Frequency (MHz)
 */
const uint32_t HSI_OSC_FREQ = 16;

/*
 *  System Clock Section Helper Function Definitions
 */
static void RCC_HSIConfig(uint32_t);
static void RCC_HSEConfig(uint32_t,uint32_t);


//AHB Prescaler Value
//const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
//const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

//PLL_P Coefficient Calculation
#define PLL_P_COEFF_CAL(x)		(((x / 2) - (1)))
#define PLL_P_COEFF_CAL_R(x)	(((2 * x) + (2)))

/*
 * This function can be used by externing it to any source file
 */
uint32_t System_Clock_Freq = 0;

/*
 * This function must be the first function called in the main block
 */
void System_Init(void)
{
	/* Configure Flash prefetch, Instruction cache, Data cache */
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_DCEN;

	/* Set Interrupt Group Priority */
	__NVIC_SetPriorityGrouping(4);

	/* Enable Interrupt and Exception */
	__NVIC_EnableIRQ(RCC_IRQn);
	__NVIC_EnableIRQ(NonMaskableInt_IRQn);
}


/*
 * System Oscilator Configuration --> [SYSTEM_CLOCK]
 */
StatusFlagTypeDef RCC_SysClkInit(RCC_SysClkInitTypeDef *pRCC_SysClk)
{
	/* PWR Enable */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	/* PWR Voltage Scaling 1 fhclk > 168MHz */
	PWR->CR |= PWR_CR_VOS;

	/* FLASH Latency */
	FLASH->ACR = FLASH_ACR_LATENCY_5WS;

	if(pRCC_SysClk->SYSCLKSource == SYSCLK_SRC_HSI)
	{
		RCC_HSIConfig(pRCC_SysClk->HSICalibrationValue);

		__SYS_CLK_HSI_SELECT__;

		return OK;
	}
	else if(pRCC_SysClk->SYSCLKSource == SYSCLK_SRC_HSE)
	{
		RCC_HSEConfig(pRCC_SysClk->HSEBypass,pRCC_SysClk->HSECSSONState);

		__SYS_CLK_HSE_SELECT__;

		return OK;
	}
//	else if(pRCC_SysClk->SYSCLKSource == SYS_CLK_SRC_PLL)
//	{
//		if(pRCC_SysClk->PLL.Source == PLL_SRC_HSE)
//		{
//			//VCO Limit Control
//			uint32_t vco_in,vco_out;
//
//			//HSE Enable
//			RCC_HSEConfig(pRCC_SysClk->HSEBypass,pRCC_SysClk->HSECSSONState);
//
//			//PLL Coefficient and VCO Limit Control
//			if(__PLL_COEFF_CONTROL__(pRCC_SysClk))
//			{
//				//PLL Ready Interrupt Enable
//				RCC->CIR |= RCC_CIR_PLLRDYIE;
//
//				//It is recommended to select a frequency of 2 MHz to limit PLL jitter ---> RM Page 227
//				if(pRCC_SysClk->PLL.Jitter == PLL_JIT_ON)
//				{
//					//Calculates limit condition value and system oscillator frequency
//					vco_in = (uint32_t)2;
//					vco_out = (uint32_t)((uint32_t)vco_in * (uint32_t)pRCC->pll.pll_N);
//					System_Clock_Freq = (uint32_t)((uint32_t)vco_out / (uint32_t)(pRCC->pll.pll_P));
//
//					//Before Calculate Check VCOUT Limit
//					if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))	//With PLL Jitter Configuration
//					{
//						//PLLM Calculated
//						pRCC_SysClk->PLL.M = (uint32_t)((uint32_t)pRCC->pll.pll_freq / (uint32_t)2); 	//VCO --> 2MHz
//
//				        /* Configure the main PLL clock source, multiplication and division factors. */
//						RCC->PLLCFGR = (uint32_t)(pRCC->pll.pll_M << RCC_PLLCFGR_PLLM_Pos) | (pRCC->pll.pll_N << RCC_PLLCFGR_PLLN_Pos) | \
//								 (PLL_P_COEFF_CAL(pRCC->pll.pll_P) << RCC_PLLCFGR_PLLP_Pos) | (RCC_PLLCFGR_PLLSRC_HSE);
//
//						__PLL_ENABLE__;
//
//						__SYS_CLK_PLL_SELECT__;
//
//						//Last Regulated System Master Clock Value
//						vco_in = ((uint32_t)(pRCC->pll.pll_freq / (uint32_t)(RCC->PLLCFGR & 0x3F)));
//						vco_out =  (uint32_t)((uint32_t)vco_in * (uint32_t)((RCC->PLLCFGR & 0x7FC0) >> 6U));
//						System_Clock_Freq = vco_out / (PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U)));
//
//						return OK;
//					}
//					else
//					{
//						return ERROR;
//					}
//				}
//				else if(pRCC_SysClk->PLL.Jitter == PLL_JIT_OFF)			//Not PLL Jitter Configuration
//				{
//					vco_in = (uint32_t)((uint32_t)pRCC->pll.pll_freq / (uint32_t)pRCC->pll.pll_M);
//					vco_out = (uint32_t)((uint32_t)vco_in * (uint32_t)pRCC->pll.pll_N);
//					System_Clock_Freq = (uint32_t)((uint32_t)vco_out / (uint32_t)pRCC->pll.pll_P);
//
//					//Before Calculate Check VCOUT Limit
//					if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))
//					{
//				        /* Configure the main PLL clock source, multiplication and division factors. */
//						RCC->PLLCFGR = (uint32_t)(pRCC->pll.pll_M << RCC_PLLCFGR_PLLM_Pos) | (pRCC->pll.pll_N << RCC_PLLCFGR_PLLN_Pos) | \
//								 (PLL_P_COEFF_CAL(pRCC->pll.pll_P) << RCC_PLLCFGR_PLLP_Pos) | (RCC_PLLCFGR_PLLSRC_HSE);
//
//						//PLL Enable
//						RCC->CR |= RCC_CR_PLLON;
//
//						//Is PLL Ready?
//						while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);
//
//						//Is PLL used as a system clock
//						RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);
//
//						//Is PLL used as a system clock
//						while(!(RCC->CFGR & RCC_CFGR_SWS_1));
//
//						//Last Regulated System Master Clock Value
//						vco_in = ((uint32_t)(pRCC->pll.pll_freq / (uint32_t)(RCC->PLLCFGR & 0x3F)));
//						vco_out =  (uint32_t)((uint32_t)vco_in * (uint32_t)((RCC->PLLCFGR & 0x7FC0) >> 6U));
//						System_Clock_Freq = vco_out / (PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U)));
//
//						return OK;
//					}
//					else
//					{
//						return ERROR;
//					}
//				}
//			}
//			else if((pRCC->pll.pll_src == PLL_SRC_HSE) & (pRCC->hse_state == HSE_OFF))
//			{
//				/*
//				 * 	TODO
//				 */
//			}
//		}
//		else if((pRCC->pll.pll_src == PLL_SRC_HSI) & (pRCC->hsi_state == HSI_ON))
//		{
//			//HSI Ready Interrupt Enable
//			RCC->CIR |= RCC_CIR_HSIRDYIE;
//
//			//PLL Ready Interrupt Enable
//			RCC->CIR |= RCC_CIR_PLLRDYIE;
//
//			/************************************
//			 * 		  HSI ENABLE SECTION		*
//			 ***********************************/
//			//HSI Enable
//			RCC->CR |= RCC_CR_HSION;
//
//			//Is HSI Ready?
//			while((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);
//
//			//HSI Calibrasyonu daha sonra yapılacak !!!
//
//			//VCO Limit Control
//			uint32_t vco_in,vco_out;
//
//			//RC Oscillator Freq
//			pRCC->pll.pll_freq = HSI_OSC_FREQ;
//
//			vco_in = (uint32_t)((uint32_t)pRCC->pll.pll_freq / (uint32_t)pRCC->pll.pll_M);
//			vco_out = (uint32_t)((uint32_t)vco_in * (uint32_t)pRCC->pll.pll_N);
//			System_Clock_Freq = (uint32_t)((uint32_t)vco_out / (uint32_t)pRCC->pll.pll_P);
//
//			//Before Calculate Check VCOUT Limit
//			if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))
//			{
//		        /* Configure the main PLL clock source, multiplication and division factors. */
//				RCC->PLLCFGR = (uint32_t)(pRCC->pll.pll_M << RCC_PLLCFGR_PLLM_Pos) | (pRCC->pll.pll_N << RCC_PLLCFGR_PLLN_Pos) | \
//						 (PLL_P_COEFF_CAL(pRCC->pll.pll_P) << RCC_PLLCFGR_PLLP_Pos) | (RCC_PLLCFGR_PLLSRC_HSI);
//
//				//PLL Enable
//				RCC->CR |= RCC_CR_PLLON;
//
//				//Is PLL Ready?
//				while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);
//
//				//Is PLL used as a system clock
//				RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);
//
//				//Is PLL used as a system clock
//				while(!(RCC->CFGR & RCC_CFGR_SWS_1));
//
//				//Last Regulated System Master Clock Value
//				vco_in = ((uint32_t)(pRCC->pll.pll_freq / (uint32_t)(RCC->PLLCFGR & 0x3F)));
//				vco_out =  (uint32_t)((uint32_t)vco_in * (uint32_t)((RCC->PLLCFGR & 0x7FC0) >> 6U));
//				System_Clock_Freq = vco_out / (PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U)));
//
//				return OK;
//			}
//			else
//			{
//				return ERROR;
//			}
//
//		}
//	}
	return ERROR;
}

///*
// *  @note : The desired oscillator source must be activated before setting the MCO
// */
//void MCO_Output(RCC_Handle_TypeDef *pRCC)
//{
//	if(pRCC->mco_out.mco_out_mode == MCO_MODE_1_ENABLE)
//	{
//		// GPIOA Clock Enable
//		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//
//		// PA8 AF Mode
//		GPIOA->MODER |= 0x02 << GPIO_MODER_MODER8_Pos;
//
//		// PA8 AF0 Config
//		GPIOA->AFR[1] |= (0U << GPIO_AFRH_AFSEL8_Pos);
//
//		// MCO1 Prescaler Configuration
//		RCC->CFGR |= (pRCC->mco_out.mco1_pres << RCC_CFGR_MCO1PRE_Pos);
//
//		// MCO1 Source Config
//		RCC->CFGR |= (pRCC->mco_out.mco1 << RCC_CFGR_MCO1_Pos);
//	}
//	else if(pRCC->mco_out.mco_out_mode == MCO_MODE_2_ENABLE)
//	{
//		// GPIOC Clock Enable
//		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
//
//		// PC9 AF Mode
//		GPIOC->MODER |= 0x02 << GPIO_MODER_MODER9_Pos;
//
//		// PC9 AF0 Config
//		GPIOC->AFR[1] |= (0U << GPIO_AFRH_AFSEL9_Pos);
//
//		// MCO2 Prescaler Configuration
//		RCC->CFGR |= (pRCC->mco_out.mco2_pres << RCC_CFGR_MCO2PRE_Pos);
//
//		// MCO2 Source Config
//		RCC->CFGR |= (pRCC->mco_out.mco2 << RCC_CFGR_MCO2_Pos);
//	}
//	else if(pRCC->mco_out.mco_out_mode == MCO_ALL_MODE_ENABLE)
//	{
//		//	PA8 -> MCO_1 Pin Out AF Mode | PC9 -> MCO_2 Pin Out AF Mode
//
//		// GPIOA Clock Enable
//		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN);
//
//		// PA8 & PC9 AF Mode
//		GPIOA->MODER |= (0x02 << GPIO_MODER_MODER8_Pos);
//		GPIOC->MODER |= (0x02 << GPIO_MODER_MODER9_Pos);
//
//		// PA8 & PC9 AF0 Config
//		GPIOA->AFR[1] |= (0U << GPIO_AFRH_AFSEL8_Pos);
//		GPIOC->AFR[1] |= (0U << GPIO_AFRH_AFSEL9_Pos);
//
//		// MCO1 & MCO2 Prescaler Configuration
//		RCC->CFGR |= (pRCC->mco_out.mco1_pres << RCC_CFGR_MCO1PRE_Pos) | (pRCC->mco_out.mco2_pres << RCC_CFGR_MCO2PRE_Pos);
//
//		// MCO1 & MCO2 Source Config
//		RCC->CFGR |= (pRCC->mco_out.mco1 << RCC_CFGR_MCO1_Pos) | (pRCC->mco_out.mco2 << RCC_CFGR_MCO2_Pos);
//	}
//}

///*
// * System Oscilattor Configuration --> [SYSTEM_CLOCK]
// */
//StatusFlagTypeDef System_Clock_Init(RCC_Handle_TypeDef *pRCC)
//{
//	switch(pRCC->sys_clk_src)
//	{
//		case (SYS_CLK_SRC_HSI):
//			if(pRCC->rcc_flag.HSI_RDY == OK)
//			{
//				RCC->CFGR |= (pRCC->prescaler.ahb_pres << RCC_CFGR_HPRE_Pos) | (pRCC->prescaler.apb1_pres << RCC_CFGR_PPRE1_Pos) |\
//						(pRCC->prescaler.apb2_pres << RCC_CFGR_PPRE2_Pos);
//				return OK;
//			}
//			else
//			{
//				return ERROR;
//			}
//			break;
//		case (SYS_CLK_SRC_HSE):
//			if(pRCC->rcc_flag.HSE_RDY == OK)
//			{
//				uint8_t clkAPB1,clkAPB2,clkHCLK;
//
//				if((clkAPB1 <= 42) & (clkAPB2 <= 84))
//				{
//					RCC->CFGR |= (pRCC->prescaler.ahb_pres << RCC_CFGR_HPRE_Pos) | (pRCC->prescaler.apb1_pres << RCC_CFGR_PPRE1_Pos) |\
//							(pRCC->prescaler.apb2_pres << RCC_CFGR_PPRE2_Pos);
//					return OK;
//				}
//				else
//				{
//					return ERROR;
//				}
//			}
//			else
//			{
//				return ERROR;
//			}
//			break;
//		case (SYS_CLK_SRC_PLL):
//
//			if((pRCC->pll.pll_src == PLL_SRC_HSE) & (pRCC->rcc_flag.HSE_RDY == OK) & (pRCC->rcc_flag.PLL_RDY == OK))
//			{
//				uint8_t clkAPB1,clkAPB2,clkHCLK;
//
//				if((clkAPB1 <= 42) & (clkAPB2 <= 84))
//				{
//					RCC->CFGR |= (pRCC->prescaler.ahb_pres << RCC_CFGR_HPRE_Pos) | (pRCC->prescaler.apb1_pres << RCC_CFGR_PPRE1_Pos) |\
//							(pRCC->prescaler.apb2_pres << RCC_CFGR_PPRE2_Pos);
//					return OK;
//				}
//				else
//				{
//					return ERROR;
//				}
//			}
//			else if ((pRCC->pll.pll_src == PLL_SRC_HSI) & (pRCC->rcc_flag.HSI_RDY == OK) & (pRCC->rcc_flag.PLL_RDY == OK))
//			{
//				uint8_t clkAPB1,clkAPB2,clkHCLK;
//
//				clkHCLK = (System_Clock_Freq) / (pRCC->prescaler.ahb_pres);
//				clkAPB1 =  (clkHCLK / (pRCC->prescaler.apb1_pres));
//				clkAPB2 = (clkHCLK / (pRCC->prescaler.apb2_pres));
//
//				if((clkAPB1 <= 42) & (clkAPB2 <= 84))
//				{
//					RCC->CFGR |= (pRCC->prescaler.ahb_pres << RCC_CFGR_HPRE_Pos) | (pRCC->prescaler.apb1_pres << RCC_CFGR_PPRE1_Pos) |\
//							(pRCC->prescaler.apb2_pres << RCC_CFGR_PPRE2_Pos);
//					return OK;
//				}
//				else
//				{
//					return ERROR;
//				}
//			}
//			else
//			{
//				return ERROR;
//			}
//			break;
//		default:
//			return ERROR;
//			break;
//	}
//	return ERROR;
//}
//
//
///*
// *  RCC Interrupt Handler  Function
// */
//void RCC_Handler(RCC_Handle_TypeDef *pRCC)
//{
//	if((RCC->CIR & RCC_CIR_LSIRDYF) == RCC_CIR_LSIRDYF)
//	{
//		/* Update Flag State */
//		pRCC->rcc_flag.LSI_RDY = OK;
//
//		/* Clear Flag State */
//		RCC->CIR |= RCC_CIR_LSIRDYC;
//	}
//	if((RCC->CIR & RCC_CIR_LSERDYF) == RCC_CIR_LSERDYF)
//	{
//		/* Update Flag State */
//		pRCC->rcc_flag.LSE_RDY = OK;
//
//		/* Clear Flag State */
//		RCC->CIR |= RCC_CIR_LSERDYC;
//	}
//	if((RCC->CIR & RCC_CIR_HSIRDYF) == RCC_CIR_HSIRDYF)
//	{
//		/* Update Flag State */
//		pRCC->rcc_flag.HSI_RDY = OK;
//
//		/* Clear Flag State */
//		RCC->CIR |= RCC_CIR_HSIRDYC;
//	}
//	if((RCC->CIR & RCC_CIR_HSERDYF) == RCC_CIR_HSERDYF)
//	{
//		/* Update Flag State */
//		pRCC->rcc_flag.HSE_RDY = OK;
//
//		/* Clear Flag State */
//		RCC->CIR |= RCC_CIR_HSERDYC;
//	}
//	if((RCC->CIR & RCC_CIR_PLLRDYF) == RCC_CIR_PLLRDYF)
//	{
//		/* Update Flag State */
//		pRCC->rcc_flag.PLL_RDY = OK;
//
//		/* Clear Flag State */
//		RCC->CIR |= RCC_CIR_PLLRDYC;
//	}
//	if((RCC->CIR & RCC_CIR_PLLI2SRDYF) == RCC_CIR_PLLI2SRDYF)
//	{
//		/* Update Flag State */
//		pRCC->rcc_flag.PLLI2S_RDY = OK;
//
//		/* Clear Flag State */
//		RCC->CIR |= RCC_CIR_PLLI2SRDYC;
//	}
//}


static void RCC_HSIConfig(uint32_t HSICalibrationValue)
{
	//HSI Ready Interrupt Enable
	//RCC->CIR |= RCC_CIR_HSIRDYIE;

	__HSI_ENABLE__;

	/*********************************************
	 * 	HSI Calibrasyonu daha sonra yapılacak !!!
	 *********************************************/
}
static void RCC_HSEConfig(uint32_t RCC_HSEBypass,uint32_t RCC_HSECSSONState)
{
	//HSE Ready Interrupt Enable
	//RCC->CIR |= RCC_CIR_HSERDYIE;

	if(RCC_HSECSSONState == HSE_CSSON_ENABLE)
	{
		//CSSON for Active HSE
		RCC->CR |= RCC_CR_CSSON;

		//HSE Clock BYPASS State ON/OFF
		RCC->CR |= RCC_HSEBypass;

		__HSE_ENABLE__;

		//CSSON for Active HSE
		while((RCC->CR & RCC_CR_CSSON) != RCC_CR_CSSON);
	}
	else if(RCC_HSECSSONState == HSE_CSSON_DISABLE)
	{
		//HSE Clock BYPASS State ON/OFF
		RCC->CR |= RCC_HSEBypass;

		__HSE_ENABLE__;

	}
}













