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
}
