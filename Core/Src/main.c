#include "GPIO_utilities.h"
#include "GPIO_interface.h"
#include "DMA_utilities.h"

char scr[100];
char dest[100];
void isr_test() {
	DMA_IR_STATUS st = DMA_Current_IR_Status(0);
	if(st.HF_IR){
		GPIO_WritePin(PORTA,PIN1,1);
		DMA_CLEAR_HT_IR(0);
	}
	if(st.TF_IR){
		GPIO_WritePin(PORTA,PIN0,1);
		DMA_CLEAR_TF_IR(0);
	}
}
int main(void)
{
	GPIO_EnableClk(PORTA);
	GPIO_Init(PORTA,PIN0,OUTPUT_PUSHPULL);
	GPIO_Init(PORTA,PIN1,OUTPUT_PUSHPULL);
	// initalize scr with ones
	for(unsigned char i=0; i<100; i++) {
	scr[i] = 1;
	}
	for(unsigned char i=0; i<100; i++) {
		dest[i] = 0;
	}
	DMA_Init();
	DMA_Channel_Config(0,20,scr, dest,INCR4_BURST_MODE,INCR4_BURST_MODE,BYTE_DATA_SIZE,BYTE_DATA_SIZE,1,1,
			FIFO_ONE_FOURTH_THRESHOLD,0);
	DMA_Enable_IR(0,VERY_HIGH_PRIORITY_LEVEL);
	DMA_ENABLE_TF_IR(0);
	DMA_ENABLE_HT_IR(0);
	DMA_Request_Transfer(0);
//	// ensure the completion of the process
	while(1) {
	}
	return 0;
}

