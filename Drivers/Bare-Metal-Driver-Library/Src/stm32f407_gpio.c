#include "stm32f407_gpio.h"

/*
 *	Alternate Function ve Interrupt Ayarı Kaldı
 *	Pinlerin değerleri registerlara yazılmadan önce
 *	ilgili pinin bit alanı temizlenicek ondan sonra yazılacak
 */
void GPIO_Config(GPIO_TypeDef *pGPIO , GPIOInitTypeDef *pInit)
{
	uint32_t shift_pos,mask_pos,temp;

	for(uint32_t curr_pos = 0 ; curr_pos < 16 ; curr_pos++)
	{
		shift_pos = (0x1U << curr_pos);
		mask_pos = (pInit->Pin & shift_pos);

		if(((mask_pos & pInit->Pin) >> curr_pos))	//pin matched
		{
			/*  Input and Analog Pin Configuration */

			// pull config
			temp &= ~(0x3U << (2 * curr_pos));
			temp |= (pInit->Pull << (2 * curr_pos));
			pGPIO->PUPDR = temp;

			// mode config
			temp = 0;
			temp &= ~(0x3U << (2 * curr_pos));
			temp |= ((pInit->Mode & GPIO_MODE)  << (curr_pos * 2));
			pGPIO->MODER = temp;

			/*  Output Pin Configuration */
			if(((pInit->Mode & MODE_OUTPUT) == MODE_OUTPUT) || ((pInit->Mode & MODE_AF) == MODE_AF))
			{
				//speed config
				temp = (pInit->Speed << (curr_pos * 2));
				pGPIO->OSPEEDR |= temp;

				//output type config
				temp = (((pInit->Mode & OUTPUT_TYPE) >> 4U) << curr_pos);
				pGPIO->OTYPER |= temp;

				//alternate function mode
				if((pInit->Mode & MODE_AF) == MODE_AF)
				{

				}
			}
			/*  Interrupt Pin (Input) Configuration */
			if((pInit->Mode == GPIO_MODE_IT_RISING) || (pInit->Mode == GPIO_MODE_IT_FALLING) | \
					(pInit->Mode == GPIO_MODE_IT_RISING_FALLING))
			{



			}
		}
	}
}

















































