#include "stm32f407_rcc.h"


/*
 * 	Caution This function is take only a RCC_SysClkInitTypeDef type variable
 */

#define __PLL_COEFF_CONTROL__(x)			(uint32_t)((x->PLL.M >= 2) & (x->PLL.M <= 63) 					& \
											(x->PLL.N <= 432) & (x->PLL.N >= 50) 							& \
											((x->PLL.P == PLL_P_DIV2) | (x->PLL.P == PLL_P_DIV6) 			| \
											 (x->PLL.P == PLL_P_DIV4) | (x->PLL.P == PLL_P_DIV8)))

/*
 * HSI RC Oscillator Frequency (MHz)
 */
const uint32_t HSI_OSC_FREQ = 16;

/*
 *  System Clock Section Helper Function Definitions
 */
static void RCC_HSIConfig(uint32_t);
static void RCC_HSEConfig(uint32_t,uint32_t);
static void MCO1_GPIO_Init(void);
static void MCO2_GPIO_Init(void);


//AHB Prescaler Value
//const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
//const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

//PLL_P Coefficient Calculation
#define PLL_P_COEFF_CAL(x)		(((x / 2) - (1)))
#define PLL_P_COEFF_CAL_R(x)	(((2 * x) + (2)))

/*
 * This function can be used by externing it to any source file
 */
double System_Clock_Freq = 0;

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
	__NVIC_EnableIRQ(MemoryManagement_IRQn);
	__NVIC_EnableIRQ(BusFault_IRQn);
	__NVIC_EnableIRQ(UsageFault_IRQn);
	__NVIC_EnableIRQ(SVCall_IRQn);
	__NVIC_EnableIRQ(DebugMonitor_IRQn);
	__NVIC_EnableIRQ(PendSV_IRQn);
}


/*
 * System Oscilator Configuration --> [SYSTEM_CLOCK]
 */
StatusFlagTypeDef RCC_SysClkInit(RCC_SysClkInitTypeDef *pRCC_SysClk)
{

	/* Check Null pointer */
	if (pRCC_SysClk == NULL)
    {
	   return ERROR;
  	}

	/* PWR Enable */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	/* PWR Voltage Scaling 1 fhclk > 168MHz */
	PWR->CR |= PWR_CR_VOS;

	/* FLASH Latency */
	FLASH->ACR = FLASH_ACR_LATENCY_5WS;

	/************************* 1.HSI selected as system clock ****************************/
	if(pRCC_SysClk->SYSCLKSource == SYSCLK_SRC_HSI)
	{
		RCC_HSIConfig(pRCC_SysClk->HSICalibrationValue);

		__SYS_CLK_HSI_SELECT__();

		return OK;
	}
	/************************* 2.HSE selected as system clock ****************************/
	if(pRCC_SysClk->SYSCLKSource == SYSCLK_SRC_HSE)
	{
		RCC_HSEConfig(pRCC_SysClk->HSEBypass,pRCC_SysClk->HSECSSONState);

		__SYS_CLK_HSE_SELECT__();

		return OK;
	}
	/************************* 3.PLL selected as system clock ****************************/
	if(pRCC_SysClk->SYSCLKSource == SYSCLK_SRC_PLL)
	{
		if(pRCC_SysClk->PLL.Source == PLL_SRC_HSE)
		{
			//VCO Limit Control
			double vco_in,vco_out;

			//HSE Enable
			RCC_HSEConfig(pRCC_SysClk->HSEBypass,pRCC_SysClk->HSECSSONState);

			//PLL Coefficient and VCO Limit Control
			if(__PLL_COEFF_CONTROL__(pRCC_SysClk))
			{
				//PLL Ready Interrupt Enable
				RCC->CIR |= RCC_CIR_PLLRDYIE;

				//It is recommended to select a frequency of 2 MHz to limit PLL jitter ---> RM Page 227
				if(pRCC_SysClk->PLL.Jitter == PLL_JITTER_ON)
				{
					//Calculates limit condition value and system oscillator frequency
					vco_in = 2;

					vco_out = (vco_in * (double)pRCC_SysClk->PLL.N);

					System_Clock_Freq = (vco_out / (double)(PLL_P_COEFF_CAL_R(pRCC_SysClk->PLL.P)));

					//Before Calculate Check VCOUT Limit
					if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))	//With PLL Jitter Configuration
					{
						//PLLM Calculated
						pRCC_SysClk->PLL.M = (uint32_t)((double)pRCC_SysClk->PLL.HSEFreq / vco_in); 	//VCO --> 2MHz

				        /* Configure the main PLL clock source, multiplication and division factors. */
						RCC->PLLCFGR = (uint32_t)(pRCC_SysClk->PLL.M << RCC_PLLCFGR_PLLM_Pos) | \
												 (pRCC_SysClk->PLL.N << RCC_PLLCFGR_PLLN_Pos) | \
												 (pRCC_SysClk->PLL.P << RCC_PLLCFGR_PLLP_Pos) | \
												 (RCC_PLLCFGR_PLLSRC_HSE);

						__PLL_ENABLE__();

						__SYS_CLK_PLL_SELECT__();

						//Last Regulated System Master Clock Value
						vco_in = (((double)pRCC_SysClk->PLL.HSEFreq / (double)(RCC->PLLCFGR & 0x3F)));

						vco_out =  ((double)vco_in * (double)((RCC->PLLCFGR & 0x7FC0) >> 6U));

						System_Clock_Freq = (vco_out / (double)(PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U))));

						return OK;
					}
					else
					{
						return ERROR;
					}
				}
				else if(pRCC_SysClk->PLL.Jitter == PLL_JITTER_OFF)			//Not PLL Jitter Configuration
				{
					vco_in = ((double)pRCC_SysClk->PLL.HSEFreq / (double)pRCC_SysClk->PLL.M);

					vco_out = ((double)vco_in * (double)pRCC_SysClk->PLL.N);

					System_Clock_Freq = ((double)vco_out / (double)PLL_P_COEFF_CAL_R(pRCC_SysClk->PLL.P));

					//Before Calculate Check VCOUT Limit
					if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))
					{
				        /* Configure the main PLL clock source, multiplication and division factors. */
						RCC->PLLCFGR = (uint32_t)(pRCC_SysClk->PLL.M << RCC_PLLCFGR_PLLM_Pos) 	|\
												 (pRCC_SysClk->PLL.N << RCC_PLLCFGR_PLLN_Pos) 	|\
												 ((pRCC_SysClk->PLL.P) << RCC_PLLCFGR_PLLP_Pos) |\
												 (RCC_PLLCFGR_PLLSRC_HSE);

						__PLL_ENABLE__();

						__SYS_CLK_PLL_SELECT__();

						//Last Regulated System Master Clock Value
						vco_in = (((double)pRCC_SysClk->PLL.HSEFreq / (double)(RCC->PLLCFGR & 0x3F)));

						vco_out =  ((double)vco_in * (double)((RCC->PLLCFGR & 0x7FC0) >> 6U));

						System_Clock_Freq = (double)vco_out / (double)(PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U)));

						return OK;
					}
					else
					{
						return ERROR;
					}
				}
			}
			else
			{
				return ERROR;
			}
		}
		else if((pRCC_SysClk->PLL.Source == PLL_SRC_HSI))
		{

			//VCO Limit Control
			double vco_in,vco_out;

			//HSI Ready Interrupt Enable
			RCC->CIR |= RCC_CIR_HSIRDYIE;

			//HSE Enable
			RCC_HSIConfig(pRCC_SysClk->HSICalibrationValue);

			//PLL Coefficient and VCO Limit Control
			if(__PLL_COEFF_CONTROL__(pRCC_SysClk))
			{
				//PLL Ready Interrupt Enable
				RCC->CIR |= RCC_CIR_PLLRDYIE;

				//It is recommended to select a frequency of 2 MHz to limit PLL jitter ---> RM Page 227
				if(pRCC_SysClk->PLL.Jitter == PLL_JITTER_ON)
				{
					//Calculates limit condition value and system oscillator frequency
					vco_in = 2;

					vco_out = (vco_in * (double)pRCC_SysClk->PLL.N);

					System_Clock_Freq = (vco_out / (double)(PLL_P_COEFF_CAL_R(pRCC_SysClk->PLL.P)));

					//Before Calculate Check VCOUT Limit
					if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))	//With PLL Jitter Configuration
					{
						//PLLM Calculated
						pRCC_SysClk->PLL.M = (uint32_t)((double)HSI_OSC_FREQ / vco_in); 	//VCO --> 2MHz

				        /* Configure the main PLL clock source, multiplication and division factors. */
						RCC->PLLCFGR = (uint32_t)(pRCC_SysClk->PLL.M << RCC_PLLCFGR_PLLM_Pos) | \
												 (pRCC_SysClk->PLL.N << RCC_PLLCFGR_PLLN_Pos) | \
												 (pRCC_SysClk->PLL.P << RCC_PLLCFGR_PLLP_Pos) | \
												 (RCC_PLLCFGR_PLLSRC_HSI);

						__PLL_ENABLE__();

						__SYS_CLK_PLL_SELECT__();

						//Last Regulated System Master Clock Value
						vco_in = (((double)HSI_OSC_FREQ / (double)(RCC->PLLCFGR & 0x3F)));

						vco_out =  ((double)vco_in * (double)((RCC->PLLCFGR & 0x7FC0) >> 6U));

						System_Clock_Freq = (vco_out / (double)(PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U))));

						return OK;
					}
					else
					{
						return ERROR;
					}
				}
				else if(pRCC_SysClk->PLL.Jitter == PLL_JITTER_OFF)			//Not PLL Jitter Configuration
				{
					vco_in = ((double)HSI_OSC_FREQ / (double)pRCC_SysClk->PLL.M);

					vco_out = ((double)vco_in * (double)pRCC_SysClk->PLL.N);

					System_Clock_Freq = ((double)vco_out / (double)PLL_P_COEFF_CAL_R(pRCC_SysClk->PLL.P));

					//Before Calculate Check VCOUT Limit
					if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))
					{
				        /* Configure the main PLL clock source, multiplication and division factors. */
						RCC->PLLCFGR = (uint32_t)(pRCC_SysClk->PLL.M << RCC_PLLCFGR_PLLM_Pos) 	|\
												 (pRCC_SysClk->PLL.N << RCC_PLLCFGR_PLLN_Pos) 	|\
												 ((pRCC_SysClk->PLL.P) << RCC_PLLCFGR_PLLP_Pos) |\
												 (RCC_PLLCFGR_PLLSRC_HSI);

						__PLL_ENABLE__();

						__SYS_CLK_PLL_SELECT__();

						//Last Regulated System Master Clock Value
						vco_in = (((double)HSI_OSC_FREQ / (double)(RCC->PLLCFGR & 0x3F)));

						vco_out =  ((double)vco_in * (double)((RCC->PLLCFGR & 0x7FC0) >> 6U));

						System_Clock_Freq = (double)vco_out / (double)(PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U)));

						return OK;
					}
					else
					{
						return ERROR;
					}
				}
			}
			else
			{
				return ERROR;
			}
		}
	}
	return ERROR;
}

/*
 *  @note : The desired oscillator source must be activated before setting the MCO
 */
void MCO_Output(uint32_t MCOx,uint32_t MCOSource,uint32_t MCOPrescaler)
{
	uint32_t mask;

	if(MCOx == RCC_MCO1)
	{
		MCO1_GPIO_Init();

		mask = ((MCOPrescaler | MCOSource) & 0x7600000UL);

		RCC->CFGR |= mask;
	}
	else if(MCOx == RCC_MCO2)
	{
		MCO2_GPIO_Init();

		mask = (RCC->CFGR & 0xF8000000UL);

		mask &=	(MCOPrescaler | MCOSource);

		RCC->CFGR |= mask;
	}
	else if(MCOx == (RCC_MCO1|RCC_MCO2))
	{
		MCO1_GPIO_Init();
		MCO2_GPIO_Init();
		RCC->CFGR |= (MCOPrescaler | MCOSource);
	}
}

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
void RCC_Handler(RCC_SysClkInitTypeDef *pRCC)
{
	if((RCC->CIR & RCC_CIR_LSIRDYF) == RCC_CIR_LSIRDYF)
	{
		/* Update Flag State */
		pRCC->OscState.LSI_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_LSIRDYC;
	}
	if((RCC->CIR & RCC_CIR_LSERDYF) == RCC_CIR_LSERDYF)
	{
		/* Update Flag State */
		pRCC->OscState.LSE_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_LSERDYC;
	}
	if((RCC->CIR & RCC_CIR_HSIRDYF) == RCC_CIR_HSIRDYF)
	{
		/* Update Flag State */
		pRCC->OscState.HSI_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_HSIRDYC;
	}
	if((RCC->CIR & RCC_CIR_HSERDYF) == RCC_CIR_HSERDYF)
	{
		/* Update Flag State */
		pRCC->OscState.HSE_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_HSERDYC;
	}
	if((RCC->CIR & RCC_CIR_PLLRDYF) == RCC_CIR_PLLRDYF)
	{
		/* Update Flag State */
		pRCC->OscState.PLL_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_PLLRDYC;
	}
	if((RCC->CIR & RCC_CIR_PLLI2SRDYF) == RCC_CIR_PLLI2SRDYF)
	{
		/* Update Flag State */
		pRCC->OscState.PLLI2S_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_PLLI2SRDYC;
	}
}


static void RCC_HSIConfig(uint32_t HSICalibrationValue)
{
	//HSI Ready Interrupt Enable
	RCC->CIR |= RCC_CIR_HSIRDYIE;

	RCC->CR  |= (HSICalibrationValue << RCC_CR_HSITRIM_Pos);

	__HSI_ENABLE__();

}
static void RCC_HSEConfig(uint32_t RCC_HSEBypass,uint32_t RCC_HSECSSONState)
{
	//HSE Ready Interrupt Enable
	RCC->CIR |= RCC_CIR_HSERDYIE;

	//HSE Clock BYPASS State ON/OFF
	RCC->CR |= RCC_HSEBypass;

	if(RCC_HSECSSONState == HSE_CSSON_ON)
	{
		__HSE_ENABLE__();

		//CSSON for Active HSE
		RCC->CR |= RCC_CR_CSSON;

		//CSSON for Active HSE
		while((RCC->CR & RCC_CR_CSSON) != RCC_CR_CSSON);
	}
	else if(RCC_HSECSSONState == HSE_CSSON_OFF)
	{
		__HSE_ENABLE__();
	}
}


static void MCO1_GPIO_Init(void)
{
	// GPIOA Clock Enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// PA8 AF Mode
	GPIOA->MODER |= 0x02 << GPIO_MODER_MODER8_Pos;

	// PA8 AF0 Config
	GPIOA->AFR[1] |= (0U << GPIO_AFRH_AFSEL8_Pos);
}
static void MCO2_GPIO_Init(void)
{
	// GPIOC Clock Enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	// PC9 AF Mode
	GPIOC->MODER |= 0x02 << GPIO_MODER_MODER9_Pos;

	// PC9 AF0 Config
	GPIOC->AFR[1] |= (0U << GPIO_AFRH_AFSEL9_Pos);
}










