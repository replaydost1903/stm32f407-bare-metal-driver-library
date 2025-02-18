#include "stm32f407_rcc.h"

/*
 * 	Önemli Not : Board da LSE için kristal osilatör ya da bypass mode kullanılmamıştır. Yani LSE yoktur
 * 	Timer driveri geliştirilirken output mode ile LSE için 1MHz altında %50 duty ye sahip bypass osilatör
 * 	frekansı üretilecektir.
 *
 */

//HSI RC Oscillator Frequency (MHz)
const uint16_t HSI_OSC_FREQ = 16;

//AHB Prescaler Value
//const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
//const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};


//PLL_P Coefficient Calculation
#define PLL_P_COEFF_CAL(x)		(((x / 2) - (1)))
#define PLL_P_COEFF_CAL_R(x)	(((2 * x) + (2)))

// This function can be used by externing it to any source file
uint32_t System_Clock_Freq=0;

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
 * System Oscilattor Configuration --> [SYSTEM_CLOCK]
 */
LibStatusFlag System_Osc_Config(RCC_Handle_TypeDef *pRCC)
{
	//PWR Enable
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	//PWR Voltage Scaling 1 fhclk > 168MHz
	PWR->CR |= PWR_CR_VOS;

	//FLASH Latency
	FLASH->ACR = FLASH_ACR_LATENCY_5WS;

	if(pRCC->sys_clk_src == SYS_CLK_SRC_HSI)
	{
		if(pRCC->hsi_state == HSI_ON)
		{
			//HSI Ready Interrupt Enable
			RCC->CIR |= RCC_CIR_HSIRDYIE;

			//HSI Enable
			RCC->CR |= RCC_CR_HSION;

			//Is HSI Ready?
			while((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);

			/*********************************************
			 * 	HSI Calibrasyonu daha sonra yapılacak !!!
			 *********************************************/

			//System clock is using HSI
			RCC->CFGR |= RCC_CFGR_SW_HSI;

			//Is HSI used as a system clock
			while((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS);

			System_Clock_Freq = 16;

			return OK;
		}
		else
		{
			return ERROR;
		}
	}
	else if(pRCC->sys_clk_src == SYS_CLK_SRC_HSE)
	{
		if(pRCC->hse_state == HSE_ON)
		{
			//HSE Ready Interrupt Enable
			RCC->CIR |= RCC_CIR_HSERDYIE;

			//CSSON for Active HSE
			RCC->CR |= RCC_CR_CSSON;

			//HSE Clock BYPASS State ON/OFF
			RCC->CR |= pRCC->hse_src << RCC_CR_HSEBYP_Pos;

			//HSE Enable
			RCC->CR |= RCC_CR_HSEON;

			//Is HSE Ready?
			while((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);

			//CSSON for Active HSE
			while((RCC->CR & RCC_CR_CSSON) != RCC_CR_CSSON);

			//System clock is using HSE
			RCC->CFGR |= RCC_CFGR_SW_HSE;

			//Is HSE used as a system clock
			while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_0);

			return OK;
		}
		else
		{
				return ERROR;
		}
	}
	else if((pRCC->sys_clk_src == SYS_CLK_SRC_PLL))
	{
		if((pRCC->pll.pll_src == PLL_SRC_HSE) & (pRCC->hse_state == HSE_ON))
		{
			//HSE Ready Interrupt Enable
			RCC->CIR |= RCC_CIR_HSERDYIE;

			//PLL Ready Interrupt Enable
			RCC->CIR |= RCC_CIR_PLLRDYIE;

			/************************************
			 * 		  HSE ENABLE SECTION		*
			 ***********************************/
			//CSSON for Active HSE
			RCC->CR |= RCC_CR_CSSON;

			//HSE Clock BYPASS State ON/OFF
			RCC->CR |= pRCC->hse_src << RCC_CR_HSEBYP_Pos;

			//HSE Enable
			RCC->CR |= RCC_CR_HSEON;

			//Is HSE Ready?
			while((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);

			//CSSON for Active HSE
			while((RCC->CR & RCC_CR_CSSON) != RCC_CR_CSSON);

			//PLL Disable
			RCC->CR |= (0x00U << RCC_CR_PLLON_Pos);

			//Is PLL Not Ready?
			while((RCC->CR & RCC_CR_PLLRDY) == RCC_CR_PLLRDY);

			//VCO Limit Control
			uint32_t vco_in,vco_out;

			//PLL Coefficient and VCO Limit Control
			if((pRCC->pll.pll_M >= 2) & (pRCC->pll.pll_M <= 63) & (pRCC->pll.pll_N <= 432) & (pRCC->pll.pll_N >= 50) &  \
					((pRCC->pll.pll_P == 2) | (pRCC->pll.pll_P == 4) | (pRCC->pll.pll_P == 6) | (pRCC->pll.pll_P == 8)) \
																													  )
			{
				//It is recommended to select a frequency of 2 MHz to limit PLL jitter ---> RM Page 227
				if(pRCC->pll.pll_jitter == PLL_JIT_ON)
				{
					//Calculates limit condition value and system oscillator frequency
					vco_in = (uint32_t)2;
					vco_out = (uint32_t)((uint32_t)vco_in * (uint32_t)pRCC->pll.pll_N);
					System_Clock_Freq = (uint32_t)((uint32_t)vco_out / (uint32_t)(pRCC->pll.pll_P));

					//Before Calculate Check VCOUT Limit
					if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))
					{
						//PLLM Calculated
						pRCC->pll.pll_M = (uint32_t)((uint32_t)pRCC->pll.pll_freq / (uint32_t)2); 	//VCO --> 2MHz

				        /* Configure the main PLL clock source, multiplication and division factors. */
						RCC->PLLCFGR = (uint32_t)(pRCC->pll.pll_M << RCC_PLLCFGR_PLLM_Pos) | (pRCC->pll.pll_N << RCC_PLLCFGR_PLLN_Pos) | \
								 (PLL_P_COEFF_CAL(pRCC->pll.pll_P) << RCC_PLLCFGR_PLLP_Pos) | (RCC_PLLCFGR_PLLSRC_HSE);

						//PLL Enable
						RCC->CR |= RCC_CR_PLLON;

						//Is PLL Ready?
						while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

						//Is PLL used as a system clock
						RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);

						//Is PLL used as a system clock
						while(!(RCC->CFGR & RCC_CFGR_SWS_1));

						//Last Regulated System Master Clock Value
						vco_in = ((uint32_t)(pRCC->pll.pll_freq / (uint32_t)(RCC->PLLCFGR & 0x3F)));
						vco_out =  (uint32_t)((uint32_t)vco_in * (uint32_t)((RCC->PLLCFGR & 0x7FC0) >> 6U));
						System_Clock_Freq = vco_out / (PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U)));

						return OK;
					}
					else
					{
						return ERROR;
					}
				}
				else	//Normal Main PLL Calculation
				{
					vco_in = (uint32_t)((uint32_t)pRCC->pll.pll_freq / (uint32_t)pRCC->pll.pll_M);
					vco_out = (uint32_t)((uint32_t)vco_in * (uint32_t)pRCC->pll.pll_N);
					System_Clock_Freq = (uint32_t)((uint32_t)vco_out / (uint32_t)pRCC->pll.pll_P);

					//Before Calculate Check VCOUT Limit
					if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))
					{
				        /* Configure the main PLL clock source, multiplication and division factors. */
						RCC->PLLCFGR = (uint32_t)(pRCC->pll.pll_M << RCC_PLLCFGR_PLLM_Pos) | (pRCC->pll.pll_N << RCC_PLLCFGR_PLLN_Pos) | \
								 (PLL_P_COEFF_CAL(pRCC->pll.pll_P) << RCC_PLLCFGR_PLLP_Pos) | (RCC_PLLCFGR_PLLSRC_HSE);

						//PLL Enable
						RCC->CR |= RCC_CR_PLLON;

						//Is PLL Ready?
						while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

						//Is PLL used as a system clock
						RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);

						//Is PLL used as a system clock
						while(!(RCC->CFGR & RCC_CFGR_SWS_1));

						//Last Regulated System Master Clock Value
						vco_in = ((uint32_t)(pRCC->pll.pll_freq / (uint32_t)(RCC->PLLCFGR & 0x3F)));
						vco_out =  (uint32_t)((uint32_t)vco_in * (uint32_t)((RCC->PLLCFGR & 0x7FC0) >> 6U));
						System_Clock_Freq = vco_out / (PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U)));

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
		else if((pRCC->pll.pll_src == PLL_SRC_HSI) & (pRCC->hsi_state == HSI_ON))
		{
			//HSI Ready Interrupt Enable
			RCC->CIR |= RCC_CIR_HSIRDYIE;

			//PLL Ready Interrupt Enable
			RCC->CIR |= RCC_CIR_PLLRDYIE;

			/************************************
			 * 		  HSI ENABLE SECTION		*
			 ***********************************/
			//HSI Enable
			RCC->CR |= RCC_CR_HSION;

			//Is HSI Ready?
			while((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);

			//HSI Calibrasyonu daha sonra yapılacak !!!

			//VCO Limit Control
			uint32_t vco_in,vco_out;

			//RC Oscillator Freq
			pRCC->pll.pll_freq = HSI_OSC_FREQ;

			vco_in = (uint32_t)((uint32_t)pRCC->pll.pll_freq / (uint32_t)pRCC->pll.pll_M);
			vco_out = (uint32_t)((uint32_t)vco_in * (uint32_t)pRCC->pll.pll_N);
			System_Clock_Freq = (uint32_t)((uint32_t)vco_out / (uint32_t)pRCC->pll.pll_P);

			//Before Calculate Check VCOUT Limit
			if((vco_out >= 100) & (vco_out <= 432) & (System_Clock_Freq <= 168))
			{
		        /* Configure the main PLL clock source, multiplication and division factors. */
				RCC->PLLCFGR = (uint32_t)(pRCC->pll.pll_M << RCC_PLLCFGR_PLLM_Pos) | (pRCC->pll.pll_N << RCC_PLLCFGR_PLLN_Pos) | \
						 (PLL_P_COEFF_CAL(pRCC->pll.pll_P) << RCC_PLLCFGR_PLLP_Pos) | (RCC_PLLCFGR_PLLSRC_HSI);

				//PLL Enable
				RCC->CR |= RCC_CR_PLLON;

				//Is PLL Ready?
				while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

				//Is PLL used as a system clock
				RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);

				//Is PLL used as a system clock
				while(!(RCC->CFGR & RCC_CFGR_SWS_1));

				//Last Regulated System Master Clock Value
				vco_in = ((uint32_t)(pRCC->pll.pll_freq / (uint32_t)(RCC->PLLCFGR & 0x3F)));
				vco_out =  (uint32_t)((uint32_t)vco_in * (uint32_t)((RCC->PLLCFGR & 0x7FC0) >> 6U));
				System_Clock_Freq = vco_out / (PLL_P_COEFF_CAL_R(((RCC->PLLCFGR & 0x30000) >> 16U)));

				return OK;
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
void MCO_Output(RCC_Handle_TypeDef *pRCC)
{
	if(pRCC->mco_out.mco_out_mode == MCO_MODE_1_ENABLE)
	{
		// GPIOA Clock Enable
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

		// PA8 AF Mode
		GPIOA->MODER |= 0x02 << GPIO_MODER_MODER8_Pos;

		// PA8 AF0 Config
		GPIOA->AFR[1] |= (0U << GPIO_AFRH_AFSEL8_Pos);

		// MCO1 Prescaler Configuration
		RCC->CFGR |= (pRCC->mco_out.mco1_pres << RCC_CFGR_MCO1PRE_Pos);

		// MCO1 Source Config
		RCC->CFGR |= (pRCC->mco_out.mco1 << RCC_CFGR_MCO1_Pos);
	}
	else if(pRCC->mco_out.mco_out_mode == MCO_MODE_2_ENABLE)
	{
		// GPIOC Clock Enable
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

		// PC9 AF Mode
		GPIOC->MODER |= 0x02 << GPIO_MODER_MODER9_Pos;

		// PC9 AF0 Config
		GPIOC->AFR[1] |= (0U << GPIO_AFRH_AFSEL9_Pos);

		// MCO2 Prescaler Configuration
		RCC->CFGR |= (pRCC->mco_out.mco2_pres << RCC_CFGR_MCO2PRE_Pos);

		// MCO2 Source Config
		RCC->CFGR |= (pRCC->mco_out.mco2 << RCC_CFGR_MCO2_Pos);
	}
	else if(pRCC->mco_out.mco_out_mode == MCO_ALL_MODE_ENABLE)
	{
		//	PA8 -> MCO_1 Pin Out AF Mode | PC9 -> MCO_2 Pin Out AF Mode

		// GPIOA Clock Enable
		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN);

		// PA8 & PC9 AF Mode
		GPIOA->MODER |= (0x02 << GPIO_MODER_MODER8_Pos);
		GPIOC->MODER |= (0x02 << GPIO_MODER_MODER9_Pos);

		// PA8 & PC9 AF0 Config
		GPIOA->AFR[1] |= (0U << GPIO_AFRH_AFSEL8_Pos);
		GPIOC->AFR[1] |= (0U << GPIO_AFRH_AFSEL9_Pos);

		// MCO1 & MCO2 Prescaler Configuration
		RCC->CFGR |= (pRCC->mco_out.mco1_pres << RCC_CFGR_MCO1PRE_Pos) | (pRCC->mco_out.mco2_pres << RCC_CFGR_MCO2PRE_Pos);

		// MCO1 & MCO2 Source Config
		RCC->CFGR |= (pRCC->mco_out.mco1 << RCC_CFGR_MCO1_Pos) | (pRCC->mco_out.mco2 << RCC_CFGR_MCO2_Pos);
	}
}

/*
 * System Oscilattor Configuration --> [SYSTEM_CLOCK]
 */
LibStatusFlag System_Clock_Config(RCC_Handle_TypeDef *pRCC)
{
	switch(pRCC->sys_clk_src)
	{
		case (SYS_CLK_SRC_HSI):
			if(pRCC->rcc_flag.HSI_RDY == OK)
			{
				RCC->CFGR |= (pRCC->prescaler.ahb_pres << RCC_CFGR_HPRE_Pos) | (pRCC->prescaler.apb1_pres << RCC_CFGR_PPRE1_Pos) |\
						(pRCC->prescaler.apb2_pres << RCC_CFGR_PPRE2_Pos);
				return OK;
			}
			else
			{
				return ERROR;
			}
			break;
		case (SYS_CLK_SRC_HSE):
			if(pRCC->rcc_flag.HSE_RDY == OK)
			{
				uint8_t clkAPB1,clkAPB2,clkHCLK;

				if((clkAPB1 <= 42) & (clkAPB2 <= 84))
				{
					RCC->CFGR |= (pRCC->prescaler.ahb_pres << RCC_CFGR_HPRE_Pos) | (pRCC->prescaler.apb1_pres << RCC_CFGR_PPRE1_Pos) |\
							(pRCC->prescaler.apb2_pres << RCC_CFGR_PPRE2_Pos);
					return OK;
				}
				else
				{
					return ERROR;
				}
			}
			else
			{
				return ERROR;
			}
			break;
		case (SYS_CLK_SRC_PLL):

			if((pRCC->pll.pll_src == PLL_SRC_HSE) & (pRCC->rcc_flag.HSE_RDY == OK) & (pRCC->rcc_flag.PLL_RDY == OK))
			{
				uint8_t clkAPB1,clkAPB2,clkHCLK;

				if((clkAPB1 <= 42) & (clkAPB2 <= 84))
				{
					RCC->CFGR |= (pRCC->prescaler.ahb_pres << RCC_CFGR_HPRE_Pos) | (pRCC->prescaler.apb1_pres << RCC_CFGR_PPRE1_Pos) |\
							(pRCC->prescaler.apb2_pres << RCC_CFGR_PPRE2_Pos);
					return OK;
				}
				else
				{
					return ERROR;
				}
			}
			else if ((pRCC->pll.pll_src == PLL_SRC_HSI) & (pRCC->rcc_flag.HSI_RDY == OK) & (pRCC->rcc_flag.PLL_RDY == OK))
			{
				uint8_t clkAPB1,clkAPB2,clkHCLK;

				clkHCLK = (System_Clock_Freq) / (pRCC->prescaler.ahb_pres);
				clkAPB1 =  (clkHCLK / (pRCC->prescaler.apb1_pres));
				clkAPB2 = (clkHCLK / (pRCC->prescaler.apb2_pres));

				if((clkAPB1 <= 42) & (clkAPB2 <= 84))
				{
					RCC->CFGR |= (pRCC->prescaler.ahb_pres << RCC_CFGR_HPRE_Pos) | (pRCC->prescaler.apb1_pres << RCC_CFGR_PPRE1_Pos) |\
							(pRCC->prescaler.apb2_pres << RCC_CFGR_PPRE2_Pos);
					return OK;
				}
				else
				{
					return ERROR;
				}
			}
			else
			{
				return ERROR;
			}
			break;
		default:
			return ERROR;
			break;
	}
	return ERROR;
}


/*
 *  RCC Interrupt Handler  Function
 */
void RCC_Handler(RCC_Handle_TypeDef *pRCC)
{
	if((RCC->CIR & RCC_CIR_LSIRDYF) == RCC_CIR_LSIRDYF)
	{
		/* Update Flag State */
		pRCC->rcc_flag.LSI_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_LSIRDYC;
	}
	if((RCC->CIR & RCC_CIR_LSERDYF) == RCC_CIR_LSERDYF)
	{
		/* Update Flag State */
		pRCC->rcc_flag.LSE_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_LSERDYC;
	}
	if((RCC->CIR & RCC_CIR_HSIRDYF) == RCC_CIR_HSIRDYF)
	{
		/* Update Flag State */
		pRCC->rcc_flag.HSI_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_HSIRDYC;
	}
	if((RCC->CIR & RCC_CIR_HSERDYF) == RCC_CIR_HSERDYF)
	{
		/* Update Flag State */
		pRCC->rcc_flag.HSE_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_HSERDYC;
	}
	if((RCC->CIR & RCC_CIR_PLLRDYF) == RCC_CIR_PLLRDYF)
	{
		/* Update Flag State */
		pRCC->rcc_flag.PLL_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_PLLRDYC;
	}
	if((RCC->CIR & RCC_CIR_PLLI2SRDYF) == RCC_CIR_PLLI2SRDYF)
	{
		/* Update Flag State */
		pRCC->rcc_flag.PLLI2S_RDY = OK;

		/* Clear Flag State */
		RCC->CIR |= RCC_CIR_PLLI2SRDYC;
	}
}



















