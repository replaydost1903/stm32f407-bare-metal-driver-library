#include "main.h"

/*
 * 		Interrupt Handler
 */

extern RCC_Handle_TypeDef r_obj;


void RCC_IRQHandler(void)
{
	RCC_Handler(&r_obj);
}


/*
 * 		Exception Handler
 */

//CSS Error
void NMI_Handler(void)
{
	RCC->CIR = RCC_CIR_CSSC;	//Clear CSS Error Flag
}
