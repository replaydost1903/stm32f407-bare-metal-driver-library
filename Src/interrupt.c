#include "main.h"

/*
 * 		Interrupt Handler
 */

extern RCC_SysClkInitTypeDef SysClk;		//SysClk değişkeni main dosyasından extern edilir.


void RCC_IRQHandler(void)
{
	RCC_Handler(&SysClk);
}


/*
 * 		Exception Handler
 */

//CSS Error
void NMI_Handler(void)
{
	RCC->CIR = RCC_CIR_CSSC;	//Clear CSS Error Flag
	while(1);
}

void HardFault_Handler(void)
{
	while(1);
}

void MemManage_Handler(void)
{
	while(1);
}

void BusFault_Handler(void)
{
	while(1);
}

void UsageFault_Handler(void)
{
	while(1);
}

void SVC_Handler(void)
{
	while(1);
}

void DebugMon_Handler(void)
{
	while(1);
}

void PendSV_Handler(void)
{
	while(1);
}




























































