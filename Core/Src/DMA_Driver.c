#include "DMA_utilities.h"
#include "GPIO_utilities.h"
#include "GPIO_interface.h"

void DMA_Init() {
//	enable clock for DMA2 Because DMA1 does not have memory to memory direction mode
	RCC_AHB1ENR |= 1 << 22;
}

void DMA_Channel_Config(char channel_Id,unsigned short number_of_transactions,char * src,char * dest,char memory_burst_mode,
		char peripheral_burst_mode,char memory_data_size,char peripheral_data_size,char memory_address_incremented,
		char peripheral_address_incremented,char fifo_threshold,char is_circlualr){

	/*char circular_mode,*/
	DMA_CR(channel_Id) &= ~(0x01); // reset enable

	/* select channel of the DMA using channel_Id
	0 : channel 0
	1 : channel 1
	2 : channel 2
	3 : channel 3
	4 : channel 4
	5 : channel 5
	6 : channel 6
	7 : channel 7
	 */
	DMA_CR(channel_Id) &= ~(7 << 25); // first clear the bits
	DMA_CR(channel_Id) |= (channel_Id << 25); // select a channel

	// set number_of_transactions
	DMA_NDTR(channel_Id) |= number_of_transactions; // number of data register

	// set source and destination addresses
	DMA_PAR(channel_Id) = (unsigned int)src;
	DMA_MAR(channel_Id) = (unsigned int)dest;

	/* set burst mode for memory and peripheral
	SINGLE_BURST_MODE : single transfer
	INCR4_BURST_MODE  : INCR4 (incremental burst of 4 beats)
	INCR8_BURST_MODE  : INCR8 (incremental burst of 8 beats)
	INCR16_BURST_MODE : INCR16 (incremental burst of 16 beats)
	 */
	DMA_CR(channel_Id) &= ~(15 << 21); // clear the 4 bits
	DMA_CR(channel_Id) |= (memory_burst_mode << 23); // Memory burst transfer configuration
	DMA_CR(channel_Id) |= (peripheral_burst_mode << 21); // peripheral burst transfer configuration

	/* set data size for memory and peripheral
	BYTE_DATA_SIZE      : byte (8-bit)
	HALF_WORD_DATA_SIZE : half-word (16-bit)
	WORD_DATA_SIZE      : word (32-bit)
	RESERVED_DATA_SIZE  : reserved
	*/
	DMA_CR(channel_Id) &= ~(15 << 11); // clear the 4 bits
	DMA_CR(channel_Id) |= (memory_data_size << 13); // Memory data size
	DMA_CR(channel_Id) |= (peripheral_data_size << 11); // peripheral data size

	/* set increment mode for memory and peripheral
	0: address pointer is fixed
	1: address pointer is incremented after each data transfer
	*/
	DMA_CR(channel_Id) &= ~(2 << 9); // clear the 4 bits
	DMA_CR(channel_Id) |= (memory_address_incremented << 9); // p increment
	DMA_CR(channel_Id) |= (peripheral_address_incremented << 10); // m increment

	//set Data transfer direction(memory to memory)
	DMA_CR(channel_Id) &= ~(3 << 6); // clear the two bits
	DMA_CR(channel_Id) |= (2 << 6); // Data transfer direction

	/* FIFO threshold selection
	FIFO_ONE_FOURTH_THRESHOLD   : 1/4 full FIFO
	FIFO_HALF_THRESHOLD         : 1/2 full FIFO
	FIFO_THREE_FOURTH_THRESHOLD : 3/4 full FIFO
	FIFO_FULL_THRESHOLD         : full FIFO
	*/
	DMA_FCR(channel_Id) &= ~(3 << 0); // clear the 2 bits
	DMA_FCR(channel_Id) |= (fifo_threshold << 0); // set the threshold

	/* set Circular mode
	0: Circular mode disabled
	1: Circular mode enabled
	*/
	DMA_CR(channel_Id) &= ~(1 << 8); // clear the bit
	DMA_CR(channel_Id) |= (is_circlualr << 8); // set the threshold

}

void DMA_Enable_IR(int channel_Id,char priority_level){
	/* set Priority level
	LOW_PRIORITY_LEVEL       : Low
	MEDIUM_PRIORITY_LEVEL    : Medium
	HIGH_PRIORITY_LEVEL      : High
	VERY_HIGH_PRIORITY_LEVEL : Very high
	*/
	DMA_CR(channel_Id) &= ~(3 << 16);	//	clear the bit
	DMA_CR(channel_Id) |=  (priority_level << 16);	//	Priority level

	// enable NVIC line for the corresponding channel
	int channels_positions[8] = {56,57,58,59,60,68,69,70}; // positions for DMA2 streams
	if((channels_positions[channel_Id] >= 32) && (channels_positions[channel_Id] <= 63)) {
		NVIC_ISER1 |= (1 << (channels_positions[channel_Id] - 32));
	}else {
		NVIC_ISER2 |= (1 << (channels_positions[channel_Id] - 32));
	}

}

void DMA_ENABLE_TF_IR(int channel_Id){
	DMA_CR(channel_Id) |= (1 << 4); // Transfer complete interrupt enable
}
void DMA_CLEAR_TF_IR(int channel_Id){
	int bits[4] = {5,11,21,27};// positions for bits in the interrupt clear reg for Transfer complete
	if(channel_Id <= 3){ //check if its low or high
		DMA_LIFCR |= (1 << bits[channel_Id]);
	}else {
		DMA_HIFCR |= (1 << bits[channel_Id]);
	}
}

void DMA_ENABLE_HT_IR(int channel_Id){
	DMA_CR(channel_Id) |= (1 << 3); //  Half transfer interrupt enable
}
void DMA_CLEAR_HT_IR(int channel_Id){
	int bits[4] = {4,10,20,26};// positions for bits in the interrupt clear reg for half transfer
	if(channel_Id <= 3 ){ //check if its low or high
		DMA_LIFCR |= (1 << bits[channel_Id]);
	}else {
		DMA_HIFCR |= (1 << bits[channel_Id]);
	}
}

void DMA_ENABLE_ERROR_IR(int channel_Id){
	DMA_CR(channel_Id) |= (1 << 2); // Transfer error interrupt enable
}
void DMA_CLEAR_ERROR_IR(int channel_Id){

	int bits[4] = {3,9,19,25};// positions for bits in the interrupt clear reg for error
	if(channel_Id <= 3 ){ //check if its low or high
		DMA_LIFCR |= (1 << bits[channel_Id]);
	}else {
		DMA_HIFCR |= (1 << bits[channel_Id]);
	}

}

DMA_IR_STATUS DMA_Current_IR_Status(int channel_Id){
	unsigned int error_bits[4] = {3,9,19,25};
	unsigned int HT_bits[4] = {4,10,20,26};
	unsigned int TF_bits[4] = {5,11,21,27};
	unsigned int error_bit = 0;
	unsigned int HF_bit = 0;
	unsigned int TF_bit = 0;
	DMA_IR_STATUS status ;
	if(channel_Id <= 3){
		error_bit = (DMA_LISR >> error_bits[channel_Id]) & 1;
		status.ERROR_IR = error_bit;
		HF_bit = (DMA_LISR >> HT_bits[channel_Id]) & 1;
		status.HF_IR = HF_bit;
		TF_bit = (DMA_LISR >> TF_bits[channel_Id]) & 1;
		status.TF_IR = TF_bit;
	}else {
		error_bit = (DMA_HISR >> error_bits[channel_Id]) & 1;
		status.ERROR_IR = error_bit;
		HF_bit = (DMA_HISR >> HT_bits[channel_Id]) & 1;
		status.HF_IR = HF_bit;
		TF_bit = (DMA_HISR >> TF_bits[channel_Id]) & 1;
		status.TF_IR = TF_bit;
	}
	return status;
}
void DMA_Request_Transfer(int channel_Id) {
	DMA_CR(channel_Id) |= (1 << 0); // start transfer
}

