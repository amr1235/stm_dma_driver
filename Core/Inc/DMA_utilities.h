#ifndef __DMA_UTILITIES__
#define __DMA_UTILITIES__

//configuration register Macro
#define DMA_CR(ch) *((unsigned int*)(0x40026400+0x0010 + 0x0018 * ch))

//peripheral address register Macro
#define DMA_PAR(ch) *((unsigned int*)(0x40026400+0x0018 + 0x0018 * ch))

//memory address register Macro
#define DMA_MAR(ch) *((unsigned int*)(0x40026400+0x001C + 0x0018 * ch))

//number of data register Macro
#define DMA_NDTR(ch) *((unsigned int*)(0x40026400+0x0014 + 0x0018 * ch))

//number of data register Macro
#define DMA_FCR(ch) *((unsigned int*)(0x40026400+0x0024 + 0x0024 * ch))

#define RCC_AHB1ENR   *((unsigned int*)(0x40023800+0x0030))

// burst modes
#define SINGLE_BURST_MODE (char)0
#define INCR4_BURST_MODE  (char)1
#define INCR8_BURST_MODE  (char)2
#define INCR16_BURST_MODE (char)3

// data sizes
#define BYTE_DATA_SIZE (char)0
#define HALF_WORD_DATA_SIZE (char)1
#define WORD_DATA_SIZE (char)2
#define RESERVED_DATA_SIZE (char)3

// FIFO threshold options
#define FIFO_ONE_FOURTH_THRESHOLD (char)0
#define FIFO_HALF_THRESHOLD (char)1
#define FIFO_THREE_FOURTH_THRESHOLD (char)2
#define FIFO_FULL_THRESHOLD (char)1

// priority Levels
#define LOW_PRIORITY_LEVEL (char)0
#define MEDIUM_PRIORITY_LEVEL (char)1
#define HIGH_PRIORITY_LEVEL (char)2
#define VERY_HIGH_PRIORITY_LEVEL (char)3

//// interrupt status
//#define TF_IR 0
//#define HF_IR 1
//#define ERROR_IR 2

// low interrupt registers
#define DMA_LISR *((unsigned int*)(0x40026400+0x0000)) // status
#define DMA_LIFCR *((unsigned int*)(0x40026400+0x0008)) // clear

// high interrupt registers
#define DMA_HISR *((unsigned int*)(0x40026400+0x0004)) // status
#define DMA_HIFCR *((unsigned int*)(0x40026400+0x000C)) // clear
// isr
#define RCC_APB2ENR		*((unsigned int*)(0x40023800+0x44))

// NVIC DMA Lines registers
#define NVIC_ISER1		*((unsigned int*)(0xE000E100 + 0x04 * 1))
#define NVIC_ISER2		*((unsigned int*)(0xE000E100 + 0x04 * 2))

// struct for IR status
typedef struct {
	unsigned int TF_IR;
	unsigned int HF_IR;
	unsigned int ERROR_IR;
}DMA_IR_STATUS;


// functions
void DMA_Channel_Config(char channel_Id,unsigned short number_of_transactions,char * src,char * dest,char memory_burst_mode,
		char peripheral_burst_mode,char memory_data_size,char peripheral_data_size,char memory_address_incremented,
		char peripheral_address_incremented,char fifo_threshold,char is_circlualr);
void DMA_Init();
void DMA_Request_Transfer();
void DMA_Clear_ISR();
void DMA_Enable_IR(int channel_Id,char priority_level);
void DMA_ENABLE_TF_IR(int channel_Id);
void DMA_CLEAR_TF_IR(int channel_Id);
void DMA_ENABLE_HT_IR(int channel_Id);
void DMA_CLEAR_HT_IR(int channel_Id);
void DMA_ENABLE_ERROR_IR(int channel_Id);
void DMA_CLEAR_ERROR_IR(int channel_Id);
DMA_IR_STATUS DMA_Current_IR_Status(int channel_Id);

#endif
