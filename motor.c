/*
 * ECE 153B
 *
 * Name(s): Tishya Chauhan, Hannah Walker
 * Section: Wednesday, Tuesday
 * Project
 */
 
 // done -> copypasted from part a

#include "stm32l476xx.h"
#include "motor.h"

static const uint32_t MASK = (GPIO_ODR_OD5|GPIO_ODR_OD6|GPIO_ODR_OD8|GPIO_ODR_OD9);
//TODO -> done i think?? im confused ab what the point of this mask is ngl
// -> purpose is to clear all pins

static const uint32_t HalfStep[8] = { // B'B0A'A00000
	0b1000100000, // B'=1 , B=0 , A'=0 , A=1 
	0b0000100000, // B'=0 , B=0 , A'=0 , A=1
	0b0100100000, // B'=0 , B=1 , A'=0 , A=1
	0b0100000000, // B'=0 , B=1 , A'=0 , A=0
	0b0101000000, // B'=0 , B=1 , A'=1 , A=0
	0b0001000000, // B'=0 , B=0 , A'=1 , A=0
	0b1001000000, // B'=1 , B=0 , A'=1 , A=0
	0b1000000000  // B'=1 , B=0 , A'=0 , A=0
}; //TODO -> done, i think?

static volatile int8_t dire = 0;
// -> (0) = not moving, (1) = clockwise, (2) = counterclockwise

static volatile uint8_t step = 0;

void Motor_Init(void) {	
	//TODO -> Done?? (its old hannah code lol)
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; //intialize port c clock
	
	//PC5 = A
	GPIOC->MODER &=~(3UL<<10); //clearing
	GPIOC->MODER |= 1UL<<10; //setting mode to output (01)
	GPIOC->OTYPER &=~(1UL<<5); //setting to push-pull (0)
	GPIOC->PUPDR &=~(3UL<<10); //setting to no PU/PD (00)
	GPIOC->OSPEEDR &=~(3UL<<10); //clearing
	GPIOC->OSPEEDR |=(1UL<<11); //setting to fast (10)

	//PC6 = A'
	GPIOC->MODER &=~(3UL<<12); //clearing
	GPIOC->MODER |= 1UL<<12; //setting mode to output (01)
	GPIOC->OTYPER &=~(1UL<<6); //setting to push-pull (0)
	GPIOC->PUPDR &=~(3UL<<12); //setting to no PU/PD (00)
	GPIOC->OSPEEDR &=~(3UL<<12); //clearing
	GPIOC->OSPEEDR |=(1UL<<13); //setting to fast (10)

	//PC8 = B
	GPIOC->MODER &=~(3UL<<16); //clearing
	GPIOC->MODER |= 1UL<<16; //setting mode to output (01)
	GPIOC->OTYPER &=~(1UL<<8); //setting to push-pull (0)
	GPIOC->PUPDR &=~(3UL<<16); //setting to no PU/PD (00)
	GPIOC->OSPEEDR &=~(3UL<<16); //clearing
	GPIOC->OSPEEDR |=(1UL<<17); //setting to fast (10)

	//PC9 = B'
	GPIOC->MODER &=~(3UL<<18); //clearing
	GPIOC->MODER |= 1UL<<18; //setting mode to output (01)
	GPIOC->OTYPER &=~(1UL<<9); //setting to push-pull (0)
	GPIOC->PUPDR &=~(3UL<<18); //setting to no PU/PD (00)
	GPIOC->OSPEEDR &=~(3UL<<18); //clearing
	GPIOC->OSPEEDR |=(1UL<<19); //setting to fast (10)
	
}

void rotate(void) {
	//TODO -> Done?
	GPIOC->ODR &=~ MASK; //clearing all odr bits with mask
	GPIOC->ODR |= HalfStep[step];
	if (dire==2){ // if command says counter clockwise
		if (step==0){
			step = 7;
		}
		else{
		--step;
		}
	}
	if (dire==1){ // if command says clockwise
		if (step==7){
			step = 0;
		}
		else{
		++step;
		}
	}
}

void setDire(int8_t direction) {
	//TODO -> Done?
	dire = direction;
}