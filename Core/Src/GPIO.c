
#include "GPIO_utilities.h"

unsigned int* GPIO[NUMBER_OF_PORTS] = {
	(unsigned int*)GPIOA_BASE_ADDRESS,
	(unsigned int*)GPIOB_BASE_ADDRESS
};


void GPIO_EnableClk(unsigned char portID)
{
	RCC_AHB1ENR |= (1 <<portID);
}

void GPIO_Init(unsigned char portID, unsigned char PIN_NO, unsigned char PIN_Dir)
{
	
		// clear this bit field (MODER)
		*(GPIO[portID]+0) &= ~(0x03 << (2*PIN_NO));
		// assign value for the bit field
		*(GPIO[portID]+0) |= (PIN_Dir>>4) <<(2*PIN_NO); // 0x10


		if((PIN_Dir>>4) == 1) /* output */
		{
			// clear the bit field (OTYPER)
			*(GPIO[portID]+1) &= ~(1 << PIN_NO);
			// assign value for the bit field
			*(GPIO[portID]+1) |= (PIN_Dir & 0x0f) <<PIN_NO;
		}

		else if ((PIN_Dir>>4) == 1)  /* input */
		{
			// clear bit field (PUPDR)
			*(GPIO[portID]+3) &= ~(3 << (PIN_NO*2));
			// assign value for the bit field 
			*(GPIO[portID]+3) |= (PIN_Dir & 0x0f) <<(PIN_NO*2);
		}
		else
		{
			// error
		}
		
}

void GPIO_WritePin(unsigned char portID, unsigned char PIN_NO, unsigned char Data)
{
	// filtering Data
	if(Data != 0) { Data=1; }

	
	// clear bits (ODR)
	*(GPIO[portID]+5) &= ~(1<< PIN_NO);
	// assign "Data" to bits
	*(GPIO[portID]+5) |= (Data << PIN_NO);
	
}

unsigned char GPIO_ReadPin(unsigned char portID, unsigned char PIN_NO)
{
	unsigned char PinState = 255;

	PinState = ( *(GPIO[portID]+4) & (1<<PIN_NO) ) >> PIN_NO;

	return PinState;
}













