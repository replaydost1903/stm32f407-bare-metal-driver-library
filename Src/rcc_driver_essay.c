#include "main.h"

RCC_SysClkInitTypeDef SysClk={0};
RCC_SysBusInitTypeDef BusClk={0};

static void System_Clock_Init(void);
void Error_Handler(void);
static void LED_Config(void);
static void LED_Toggle(void);

int main()
{
	System_Init();

	MCO_Output(RCC_MCO1|RCC_MCO2, MCO1_SRC_HSE|MCO2_SRC_HSE, MCO1_DIV_2|MCO2_DIV_2);

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
	SysClk.SYSCLKSource = SYSCLK_SRC_HSE;
	SysClk.HSEBypass = HSE_NOT_BYPASS;
	SysClk.HSECSSONState = HSE_CSSON_ON;
	SysClk.PowerSavingMode = RCC_HSI_POWER_SAVE | RCC_PLL_POWER_SAVE;
	SysClk.PLL.Source = PLL_SRC_HSE;
	SysClk.PLL.Jitter = PLL_JITTER_OFF;
	SysClk.PLL.M = 4;
	SysClk.PLL.N = 168;
	SysClk.PLL.P = PLL_P_DIV2;

	if((RCC_SysClkInit(&SysClk) != OK))
	{
		Error_Handler();
	}

	BusClk.SYSCLKSource = SYSCLK_SRC_HSE;
	BusClk.AHBPrescaler = RCC_AHB_DIV_1;
	BusClk.APB1Prescaler = RCC_APB1_DIV_4;
	BusClk.APB2Prescaler = RCC_APB2_DIV_2;

	if((RCC_SysBusInit(&BusClk) != OK))
	{
		Error_Handler();
	}
}
void Error_Handler()
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
	GPIOD->ODR ^= (0x1U << 13U) | (0x1U << 12U);
	for(uint32_t i=0;i<1000000;i++);
}








