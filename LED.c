/*
 * ECE 153B
 *
 * Name(s): Tishya Chauhan, Hannah Walker
 * Section: Wednesday, Tuesday
 * Project
 */

#include "LED.h"

// done?? copypasted from past lab

void LED_Init(void){
	//TODO -> Done

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable port A clock
	
	// initialize green LED
	GPIOA->MODER &=~ (3UL<<10); // shift by 10
	GPIOA->MODER |= 1UL<<10; // (01) set to output
	GPIOA->OTYPER &=~ (1UL<<5); // (0) set output to push pull
	GPIOA->PUPDR &=~ (3UL<<10); // (00) no PUPD
}

void LED_On(void){
	//TODO -> Done
	GPIOA->ODR |= GPIO_ODR_OD5;
}

void LED_Off(void){
	//TODO -> Done
	GPIOA->ODR &=~ GPIO_ODR_OD5;
}

void LED_Toggle(void){
	//TODO -> Done
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
