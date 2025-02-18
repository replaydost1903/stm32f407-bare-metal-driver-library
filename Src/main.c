#include "main.h"


/****************************************************
 ******APB1 APB2 AHB Clock Hızları Ayarlanılacak*****
 ****************************************************/


//Peripheral Data Type
RCC_Handle_TypeDef r_obj = {0};
GPIOInitTypeDef g_obj = {0};

//Function Declarations
static void LED_Config(void);
static void LED_Toggle(void);
static void RCC_Init(RCC_Handle_TypeDef*);
static void GPIO_Init(void);
void Error_Handler(void);

//Extern
extern uint32_t System_Clock_Freq;


int main(void)
{
	/*System Config*/
	System_Init();

	/*Clock Config*/
	RCC_Init(&r_obj);

	/*MCO Config*/
	MCO_Output(&r_obj);

	/*GPIO Init*/
	GPIO_Init();

	LED_Config();

	while(1)
	{
		LED_Toggle();
	}
}

/*
 *  RCC Peripheral Init
 */
static void RCC_Init(RCC_Handle_TypeDef* osc)
{
	/*
	 *	System Oscillator Configuration Section
	 */
	osc->sys_clk_src = SYS_CLK_SRC_PLL;
	osc->hse_state = HSE_ON;
	osc->hsi_state = HSI_OFF;
	osc->hse_src = HSE_NOT_BYPASS;
	osc->pll.pll_M = 4;
	osc->pll.pll_N = 168;
	osc->pll.pll_P = PLL_P_2;
	osc->pll.pll_Q = 4;
	osc->pll.pll_freq = 8;
	osc->pll.pll_src = PLL_SRC_HSE;
	osc->pll.pll_jitter = PLL_JIT_ON;


	if(System_Osc_Config(&r_obj) != OK)
	{
		Error_Handler();
	}

	/*
	 *  System Clock Configuration Section
	 */
	osc->prescaler.ahb_pres = AHB_PRESCALER_1;
	osc->prescaler.apb1_pres = APB1_PRESCALER_4;
	osc->prescaler.apb2_pres = APB2_PRESCALER_2;

	if(System_Clock_Config(&r_obj) != OK)
	{
		Error_Handler();
	}
}

void Error_Handler(void)
{
	while(1);
}

static void LED_Config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= (0x1U << 30U) | (0x1U << 28U) | (0x1U << 26U) | (0x1U << 24U);
}
static void LED_Toggle(void)
{
	GPIOD->ODR ^= (0x1U << 15U) | (0x1U << 14U) | (0x1U << 13U) | (0x1U << 12U);
	for(uint32_t i=0;i<1000000;i++);
}

static void GPIO_Init(void)
{
	g_obj.Pin = GPIO_PIN_0 | GPIO_PIN_5 | GPIO_PIN_15;
	g_obj.Mode = GPIO_MODE_OUTPUT_OD;
	g_obj.Pull = GPIO_PULL_UP;
	g_obj.Speed = GPIO_SPEED_VERY_HIGH;

	GPIO_Config(GPIOA, &g_obj);
}












