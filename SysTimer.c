/*
 * ECE 153B
 *
 * Name(s): Tishya Chauhan, Hannah Walker
 * Section: Wednesday, Tuesday
 * Project
 */
 
 // done? mostly copypasted from part a

#include "SysTimer.h"
#include "motor.h"

static uint32_t volatile step = 0;
static uint32_t volatile stop_time = 0;

void SysTick_Init(void) {
	// SysTick Control & Status Register
	SysTick->CTRL = 0; // Disable SysTick IRQ and SysTick Counter

	SysTick->LOAD = 79999;
	SysTick->VAL = 0;
	
	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 
	// Select clock source
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		
	
	// Enable SysTick IRQ and SysTick Timer
	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1
}

void SysTick_Handler(void) {
	//TODO -> done? copypasted from part a
	++step;
	//rotate();
	
	//TRIAL
	if (step >= stop_time && stop_time != 0) {
		setDire(0);      // stop after timeout
		stop_time = 0;
	} else {
		rotate();
	}

}

void delay(uint32_t ms) {
	//TODO -> done? copypasted from part a
	step = 0;
	(SysTick->VAL) = 0;
	(SysTick->LOAD) = 79999; // since clock is 80MHz and we want ms
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while (step < ms); // delay in ms
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk; // disable SysTick for future use
}