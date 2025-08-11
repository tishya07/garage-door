/*
 * ECE 153B
 *
 * Name(s): Tishya Chauhan, Hannah Walker
 * Section:
 * Project
 */
 
 //DONE
 
#include "DMA.h"
#include "SysTimer.h"
#include "UART.h"

void DMA_Init_UARTx(DMA_Channel_TypeDef * tx, USART_TypeDef * uart) {
	//TODO
	//enable clock for DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	if (tx == DMA1_Channel7){
		
		//channel 7 uart2_tx
		// disable the channel
		DMA1_Channel7->CCR &= ~DMA_CCR_EN; 
		// disable memory-to-memory mode
		DMA1_Channel7->CCR &= ~DMA_CCR_MEM2MEM;
		// set channel priority to high
		DMA1_Channel7->CCR &= ~DMA_CCR_PL;
		DMA1_Channel7->CCR |= DMA_CCR_PL_1;
		// set peripheral size to 8-bit
		DMA1_Channel7->CCR &= ~DMA_CCR_PSIZE;
		// set memory size to 8-bit
		DMA1_Channel7->CCR &= ~DMA_CCR_MSIZE;
		// disable peripheral incremement mode
		DMA1_Channel7->CCR &= ~DMA_CCR_PINC;
		// enable memory increment mode
		DMA1_Channel7->CCR |= DMA_CCR_MINC;
		// disable circular mode
		DMA1_Channel7->CCR &= ~DMA_CCR_CIRC;
		// set data transfer direction to memory-to-peripheral
		DMA1_Channel7->CCR |= DMA_CCR_DIR;

		DMA1_CSELR->CSELR &=~ DMA_CSELR_C7S;
		DMA1_CSELR->CSELR |= 1UL<<25; // 11.5.7 (0010) mapped to USART2_TX
		
		// SETUP UP DMA INTERRUPTS
		// disable half transfer interrupt
		DMA1_Channel7->CCR &= ~DMA_CCR_HTIE;
		// disable transfer error interrupt
		DMA1_Channel7->CCR &= ~DMA_CCR_TEIE;
		// enable transfer complete interrupt
		DMA1_Channel7->CCR |= DMA_CCR_TCIE;
		// set interrupt priority to 0 in NVIC
		NVIC_SetPriority(DMA1_Channel7_IRQn, 0);
		// enable interrupt in NVIC
		NVIC_EnableIRQ(DMA1_Channel7_IRQn);
	
	} else {
	
		//channel 4 uart1_tx
		// disable the channel
		DMA1_Channel4->CCR &= ~DMA_CCR_EN; 
		// disable memory-to-memory mode
		DMA1_Channel4->CCR &= ~DMA_CCR_MEM2MEM;
		// set channel priority to high
		DMA1_Channel4->CCR &= ~DMA_CCR_PL;
		DMA1_Channel4->CCR |= DMA_CCR_PL_1;
		// set peripheral size to 8-bit
		DMA1_Channel4->CCR &= ~DMA_CCR_PSIZE;
		// set memory size to 8-bit
		DMA1_Channel4->CCR &= ~DMA_CCR_MSIZE;
		// disable peripheral incremement mode
		DMA1_Channel4->CCR &= ~DMA_CCR_PINC;
		// enable memory increment mode
		DMA1_Channel4->CCR |= DMA_CCR_MINC;
		// disable circular mode
		DMA1_Channel4->CCR &= ~DMA_CCR_CIRC;
		// set data transfer direction to memory-to-peripheral
		DMA1_Channel4->CCR |= DMA_CCR_DIR;

		DMA1_CSELR->CSELR &=~ DMA_CSELR_C4S;
		DMA1_CSELR->CSELR |= 1UL<<13; // 11.5.7 (0010) mapped to USART1_TX
		
		// SETUP UP DMA INTERRUPTS
		// disable half transfer interrupt
		DMA1_Channel4->CCR &= ~DMA_CCR_HTIE;
		// disable transfer error interrupt
		DMA1_Channel4->CCR &= ~DMA_CCR_TEIE;
		// enable transfer complete interrupt
		DMA1_Channel4->CCR |= DMA_CCR_TCIE;
		// set interrupt priority to 0 in NVIC
		NVIC_SetPriority(DMA1_Channel4_IRQn, 0);
		// enable interrupt in NVIC
		NVIC_EnableIRQ(DMA1_Channel4_IRQn);
		
	}
	
}

void DMA1_Channel7_IRQHandler(void){
	// check transfer complete interrupt flag
	// -> if flag occurs, implement 
	if (DMA1->ISR & DMA_ISR_TCIF7){
		DMA1->IFCR |= DMA_IFCR_CTCIF7;
	
		USART2->CR1 |= USART_CR1_TCIE;
	}
	// clear global DMA interrupt flag
	DMA1->IFCR |= DMA_IFCR_CGIF7;
	
	// clear NVIC interrupt flag
	NVIC_ClearPendingIRQ(DMA1_Channel7_IRQn);
	

}

void DMA1_Channel4_IRQHandler(void){
	// check transfer complete interrupt flag
	// -> if flag occurs, implement 
	if (DMA1->ISR & DMA_ISR_TCIF4){
		DMA1->IFCR |= DMA_IFCR_CTCIF4;
		
		USART1->CR1 |= USART_CR1_TCIE;
	}
	// clear global DMA interrupt flag
	DMA1->IFCR |= DMA_IFCR_CGIF4;
	
	// clear NVIC interrupt flag
	NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
}
