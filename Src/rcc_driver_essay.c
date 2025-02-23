#include "main.h"

RCC_SysClkInitTypeDef SysClk;

static void System_Clock_Init(void);
void Error_Handler(void);
static void MCO1_Output_Enable(void);
static void LED_Config(void);
static void LED_Toggle(void);
//static void User_Button_Init(void);

int main()
{
	System_Init();

	MCO1_Output_Enable();

	System_Clock_Init();

	LED_Config();

	while(1)
	{
		LED_Toggle();
	}

	return 0;
}


static void System_Clock_Init()
{
	SysClk.SYSCLKSource = SYSCLK_SRC_PLL;
	SysClk.HSEBypass = HSE_NOT_BYPASS;
	SysClk.HSECSSONState = HSE_CSSON_ON;
	SysClk.PLL.Source = PLL_SRC_HSI;
	SysClk.PLL.HSEFreq = 8;
	SysClk.PLL.Jitter = PLL_JITTER_ON;
	SysClk.PLL.M = 5;
	SysClk.PLL.N = 90;
	SysClk.PLL.P = PLL_P_DIV2;

	if((RCC_SysClkInit(&SysClk) != OK))
	{
		Error_Handler();
	}

}
void Error_Handler()
{
	while(1);
}
static void MCO1_Output_Enable(void)
{
	// GPIOA Clock Enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// PA8 AF Mode
	GPIOA->MODER |= 0x02 << GPIO_MODER_MODER8_Pos;

	// PA8 AF0 Config
	GPIOA->AFR[1] |= (0U << GPIO_AFRH_AFSEL8_Pos);

	// MCO1 Prescaler Configuration
	RCC->CFGR |= (0x7U << RCC_CFGR_MCO1PRE_Pos);

	// MCO1 Source Config
	RCC->CFGR |= (0x3U << RCC_CFGR_MCO1_Pos);
}
static void LED_Config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= (0x1U << 30U) | (0x1U << 28U) | (0x1U << 26U) | (0x1U << 24U);
}
static void LED_Toggle(void)
{
	GPIOD->ODR ^= (0x1U << 13U) | (0x1U << 12U);
	for(uint32_t i=0;i<1000000;i++);
}
//static void User_Button_Init(void)
//{
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//	GPIOA->MODER |= 0x00000000;
//	GPIOA->PUPDR |= 0x00000000;
//}








