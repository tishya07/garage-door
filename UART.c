/*
 * ECE 153B
 *
 * Name(s): Tishya Chauhan, Hannah Walker
 * Section: Wednesday, Tuesday
 * Project
 */

// NOT DONE, but added some code

#include "UART.h"
#include "DMA.h"
#include "SysTimer.h"
#include <string.h>
#include <stdio.h>

static char debug[IO_SIZE];

static volatile DMA_Channel_TypeDef * tx;
static volatile char inputs[IO_SIZE];
static volatile uint8_t data_t_0[IO_SIZE];
static volatile uint8_t data_t_1[IO_SIZE];
static volatile uint8_t input_size = 0;
static volatile uint8_t pending_size = 0;
static volatile uint8_t * active = data_t_0;
static volatile uint8_t * pending = data_t_1;

#define SEL_0 1
#define BUF_0_EMPTY 2
#define BUF_1_EMPTY 4
#define BUF_0_PENDING 8
#define BUF_1_PENDING 16

void transfer_data(char ch);
void on_complete_transfer(void);

void UART1_Init(void) {
//TODO -> Done (maybe)
	// REF 8.4
	// enable the USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //enable gpio clock
	
	//select the system clock as the USART1 clock source in the peripheral independent config register
	RCC->CCIPR &=~(RCC_CCIPR_USART1SEL); //clearing clock selection
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0; // (01) system clock selected as USART1 clock
	tx = DMA1_Channel4;

}

void UART2_Init(void) {
	// [TODO] -> Done (maybe)
	//REF 8.4
	// enable the USART2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	//select the system clock as the USART2 clock source in the peripheral independent clock config register
	RCC->CCIPR &=~(RCC_CCIPR_USART2SEL); //clearing clock selection
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0; // (01) system clock selected as USART2 clock
	tx=DMA1_Channel7;

}

void UART1_GPIO_Init(void) {
	// [TODO] -> Done (maybe)
	//REF 9.4
	//config PB6 as USART1_TX
	GPIOB->MODER &=~GPIO_MODER_MODE6; //clear 6
	GPIOB->MODER |= GPIO_MODER_MODE6_1; //set as AF mode (10)
	
	GPIOB->AFR[0] &=~GPIO_AFRL_AFRL6; //clearing
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL6_0 | GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL6_2); //USART1_TX is AF7 = 0111
	
	//config PB7 as USART1_RX
	GPIOB->MODER &=~GPIO_MODER_MODE7; //clear 7
	GPIOB->MODER |= GPIO_MODER_MODE7_1; //set as AF mode (10)
	
	GPIOB->AFR[0] &=~GPIO_AFRL_AFRL7; //clearing
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL7_0 | GPIO_AFRL_AFSEL7_1 | GPIO_AFRL_AFSEL7_2); //USART1_RX is AF7 = 0111
	
	//both operate at very high speed
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6; // (11) = high speed set for PB6
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7; // (11) = high speed set for PB7
	
	//both set to push/pull
	GPIOB->OTYPER &=~GPIO_OTYPER_OT6; // set 0 push pull for PB6
	GPIOB->OTYPER &=~GPIO_OTYPER_OT7; // set 0 push pull for PB7
	
	//both to pull-up
	GPIOB->PUPDR &=~ GPIO_PUPDR_PUPD6; // clearing
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0; // (01) is pull-up
	
	GPIOB->PUPDR &=~ GPIO_PUPDR_PUPD7; // clearing
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD7_0; // (01) is pull-up
	
	// set interrupt priority to 0 in NVIC
	NVIC_SetPriority(USART1_IRQn, 0);
	// enable interrupt in NVIC
	NVIC_EnableIRQ(USART1_IRQn);
}

void UART2_GPIO_Init(void) {
	// [TODO] -> Done
	//REF 9.4
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	//config PA2 as UART transmitter
	GPIOA->MODER &=~GPIO_MODER_MODE2; //clear 2
	GPIOA->MODER |= GPIO_MODER_MODE2_1; //set as AF mode (10)
	
	GPIOA->AFR[0] &=~GPIO_AFRL_AFRL2; //clearing
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2); //USART2_TX is AF7 = 0111
	
	//config PA3 as UART receiver
	
	GPIOA->MODER &=~GPIO_MODER_MODE3; //clear 3
	GPIOA->MODER |= GPIO_MODER_MODE3_1; //set as AF mode (10)
	
	GPIOA->AFR[0] &=~GPIO_AFRL_AFRL3; //clearing
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2); //USART2_RX is AF7 = 0111
	
	//both operate at very high speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2; // (11) = high speed set for PA2
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED3; // (11) = high speed set for PA3
	
	//both set to push/pull
	GPIOA->OTYPER &=~GPIO_OTYPER_OT2; // set 0 push pull for PA2
	GPIOA->OTYPER &=~GPIO_OTYPER_OT3; // set 0 push pull for PA3
	
	//config both to use pull-up for i/o
	GPIOA->PUPDR &=~ GPIO_PUPDR_PUPD2; // clearing
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0; // (01) is pull-up
	
	GPIOA->PUPDR &=~ GPIO_PUPDR_PUPD3; // clearing
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0; // (01) is pull-up
	
	// set interrupt priority to 0 in NVIC
	NVIC_SetPriority(USART2_IRQn, 0);
	// enable interrupt in NVIC
	NVIC_EnableIRQ(USART2_IRQn);
}

void USART_Init(USART_TypeDef * USARTx) {
	// TODO -> Done?? just copy pasted from past lab...
	
	// Disable USART before configuring settings
	USARTx->CR1 &= ~USART_CR1_UE;
	
	// Set Communication Parameters
	USARTx->CR1 &= ~(USART_CR1_M);     // 00 -> 8 Data Bits
	USARTx->CR1 &= ~(USART_CR1_OVER8); // 0 -> Oversampling by 16
	USARTx->CR2 &= ~(USART_CR2_STOP);  // 00 -> 1 Stop Bit
	
	// Set Baud Rate
	// f_CLK = 80 MHz, Baud Rate = 9600 = 80 MHz / DIV -> DIV = 8333 = 0x208D
	USARTx->BRR = 0x208D;
	
	// Enable Transmitter/Receiver
	USARTx->CR1 |= USART_CR1_TE | USART_CR1_RE;
	
	USARTx->CR3 |= USART_CR3_DMAT;
	
	// Enable USART
	USARTx->CR1 |= USART_CR1_UE;
	
	USARTx->CR1 |= USART_CR1_RXNEIE;
	
	USARTx->CR1 |= USART_CR1_TCIE; //added but idk if needed here CHECK
	
}

/**
 * This function accepts a string that should be sent through UART
*/
void UART_print(char* data) {
	//TODO
	uint32_t length = strlen(data);
	//Transfer char array to buffer
	
	//Check DMA status. If DMA is ready, send data
	if ((tx->CCR&DMA_CCR_EN)==0){
		for (int i=0; i<length; i++){
			active[i] = data[i];
		}
			tx->CMAR = (uint32_t) active; //CHECK
		  tx->CNDTR = length;
			tx->CPAR = (uint32_t)&(USART2->TDR);
			tx->CCR |= DMA_CCR_EN;
		  //USART2->CR1 |= USART_CR1_TCIE; 

	}
		
	//If DMA is not ready, put the data aside
	else{
		for (int i=0; i<length; i++){
			pending[i] = data[i];
		}
		pending_size = length;
	}
	
	
}

/**
 * This function should be invoked when a character is accepted through UART
*/
void transfer_data(char ch) {
	//TODO
	static int i;
	/*
	while (!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = ch;
	
	sprintf(debug, "Got char: %02X (%c)\n", ch, ch);
  UART_print(debug); */
	
	if (i < IO_SIZE - 1) {
		inputs[i] = ch;     // Accumulate character
		i++;
  } 
		input_size = i;
		inputs[i] = '\0';     // Null-terminate the string
		
		UART_onInput(inputs, input_size);   // Process command
		
		i = 0;                      // Reset for next command
		
}

/**
 * This function should be invoked when DMA transaction is completed
*/
void on_complete_transfer(void) {
	//TODO
	// If there are pending data to send, switch active and pending buffer, and send data
	if (pending_size > 0) {
        // Swap active and pending
        //uint8_t *temp = active;
        //active = pending;
        //pending = temp;
		for (int i=0; i<pending_size; i++){
			active[i] = pending[i];
		}

        // Set active size for DMA transfer
        uint32_t size = pending_size;
        pending_size = 0;

        // Set memory address and transfer count
        tx->CMAR = (uint32_t)active;
        tx->CNDTR = size;

        // Enable DMA channel again
        tx->CCR |= DMA_CCR_EN;
    }
}

void USART1_IRQHandler(void){
	//TODO -> done i think?
	// When receive a character, invoke transfer_data
	if ((USART1->ISR)&USART_ISR_RXNE){
		USART1->ICR |= USART_ICR_ORECF;
		transfer_data(USART1->RDR);
	}
	// When complete sending data, invoke on_complete_transfer
	if ((USART1->ISR)&USART_ISR_TC){
		USART1->ICR |= USART_ICR_TCCF;  // Clear the TC flag
		tx->CCR &=~ DMA_CCR_EN; // clearing the flag
		on_complete_transfer();
	}
}

void USART2_IRQHandler(void){
	//TODO
	//TODO -> done i think?
	// When receive a character, invoke transfer_data
	if (((USART2->ISR) & USART_ISR_RXNE) | (USART2->ISR & USART_ISR_ORE)){
		USART2->ICR |= USART_ICR_ORECF;
		
		char ch = USART2->RDR;
		/*
		while (!(USART2->ISR & USART_ISR_TXE));
		//USART2->TDR = ch; // echo back
		
		sprintf(debug, "Char: 0x%02X (%c)\n", ch, ch);
		UART_print(debug);
		
		sprintf(debug, "RX\n");
	  UART_print(debug); */
		
		transfer_data(USART2->RDR);
	}
	// When complete sending data, invoke on_complete_transfer
	if ((USART2->ISR)&USART_ISR_TC){
		USART2->ICR |= USART_ICR_TCCF;  // Clear the TC flag
		tx->CCR &=~ DMA_CCR_EN; 				// clearing the flag
		
		//sprintf(debug, "TC\n");
	  //UART_print(debug);
	
		on_complete_transfer();
	} 
}
