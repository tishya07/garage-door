/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "DMA.h"
#include "UART.h"
#include "motor.h"
#include "SPI.h"
#include "I2C.h"
#include "accelerometer.h"
#include <stdio.h>
#include <string.h>

static char buffer[IO_SIZE];
static uint32_t volatile stop_time = 0;
static uint32_t volatile step = 0;

void UART_onInput(char* inputs, uint32_t size) {
	//TODO -> should work ?? but might want to try this out
	// strncmp(inputs, "Clockwise", size);
	
	if (size != 1){
		sprintf(buffer, "Invalid command.\nPlease type 'O' to open, 'C' to close, and 'S' to stop the garage door.\n");
		UART_print(buffer);
	}
	if (*inputs == 'O' || *inputs == 'o'){ 
		setDire(1);
		sprintf(buffer, "The garage is now open.\n");
		UART_print(buffer);
		//delay(4000); //blocking, must be non blockng
		stop_time = step + 4000;
	}
	if (*inputs == 'C' || *inputs == 'c'){
		
		sprintf(buffer, "The garage is now closed.\n");
		UART_print(buffer);
		setDire(2);
		//delay(4000); //blocking, must be non blockng
		stop_time = step + 4000;
	}
	if (*inputs == 'S' || *inputs == 's'){
		setDire(0);
		sprintf(buffer, "Motor is now turned off.\n");
		UART_print(buffer);
		//delay(4000); //blocking, must be non blockng
		stop_time = step + 4000;
	}
}

int main(void) {
	//int count = 1000;
	// Switch System Clock = 80 MHz
	//TERMITE: UART2 DMA CHANNEL 7
	//BLUETOOTH: UART1 DMA CHANNEL 4
	System_Clock_Init(); 
	Motor_Init();
	SysTick_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART1);
	LED_Init();	
	DMA_Init_UARTx(DMA1_Channel4, USART1);
	SPI1_GPIO_Init();
	SPI1_Init();
	initAcc();
	I2C_GPIO_Init();
	I2C_Initialization();
	
	//TEST MOTOR
	/*setDire(1);

	while (1) {
			rotate();
			delay(1000); // tune this for speed
	}
	*/
	
	//TEST RXNE FLAG
	/*if (USART2->ISR & USART_ISR_RXNE) {
            char c = USART2->RDR;
            UART_print("Got a character!\r\n");
        }*/
	
	//TEST DMA AND UART
	/*sprintf(buffer, "Initial\r\n");
	UART_print(buffer);
	LED_Toggle();
	while (DMA1_Channel7->CCR & DMA_CCR_EN); //TIMING ISSUE, PENDING REG NEVER GETS LOADED INTO ACTIVE
	//while(count--);
	sprintf(buffer, "Final\r\n");
	UART_print(buffer);
	//while (DMA1_Channel7->CCR & DMA_CCR_EN);*/
	
	/*
	sprintf(buffer, "Program Starts.\r\n");
	UART_print(buffer);
	sprintf(buffer, "Type 'O' to open, 'C' to close, and 'S' to stop the garage door.\n");
	UART_print(buffer);
	*/

	while(1) {
		//TODO -> done?
		double x,y,z;
		
		LED_Toggle();
		delay(1000);
		
		uint8_t temp = Temp_Read();
		readValues(&x, &y, &z);
		
		//TEST ACCELOROMETER
		//sprintf(buffer, "Acceleration: %.2f, %.2f, %.2f\r\n", x, y, z);
	  //UART_print(buffer);
		
		//TEST TEMP SENSOR
		//sprintf(buffer, "Temperature: %d\n", temp);
		//UART_print(buffer);
		

		if (temp > 28){
			sprintf(buffer, "Temperature too high. Door opening.\n");
			UART_print(buffer);
			
			//if door horizontal, start motor
			if (z < 0.95){
				setDire(1);
				delay(200);
			} else {
				setDire(0);
			}
			
		} else if (temp < 26){
			sprintf(buffer, "Temperature too low. Door closing.\n");
			UART_print(buffer);
			
			//if door vertical, start motor
			if (y > -0.95){
				setDire(2);
				delay(200);
			} else {
				setDire(0);
			}
		}
		
		//print temp and acceleration
		sprintf(buffer, "Temperature: %d Acceleration: %.2f, %.2f, %.2f\r\n", temp, x, y, z);
		UART_print(buffer);
		delay(200); //blocking, must be non blockng

	}
	
}


