#include "main.h"

RCC_SysClkInitTypeDef SysClk;

static void System_Clock_Init(void);
void Error_Handler(void);
static void MCO1_Output_Enable(void);
static void LED_Config(void);
static void LED_Toggle(void);

int main()
{
	System_Init();

	System_Clock_Init();

	MCO1_Output_Enable();

	LED_Config();

	while(1)
	{
		LED_Toggle();
	}

	return 0;
}


static void System_Clock_Init()
{
	SysClk.SYSCLKSource 	= 		SYSCLK_SRC_HSI;
	//SysClk.HSEBypass 		= 		HSE_NOT_BYPASS;
	//SysClk.HSECSSONState 	= 		HSE_CSSON_ENABLE;

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
	RCC->CFGR |= (0x6 << RCC_CFGR_MCO1PRE_Pos);

	// MCO1 Source Config
	RCC->CFGR |= (0U << RCC_CFGR_MCO1_Pos);
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









