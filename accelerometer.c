#include "SPI.h"
#include "SysTimer.h"
#include "accelerometer.h"

// done?? completely copypasted from lab 4

void accWrite(uint8_t addr, uint8_t val){
	//TODO -> done? copypasted from lab 4
	SPI_Transfer_Data(((addr & 0x3F) << 8) | val);
}

uint8_t accRead(uint8_t addr){
	//TODO -> done? copypasted from lab 4
	
	// access SPI_Transfer_Data
	uint16_t read_data;
	
	addr = addr & ~(0xC0);
	
	read_data = SPI_Transfer_Data((0x80 | addr) << 8);
	
	return (read_data & 0xFF);
}

void initAcc(void){
	//TODO -> done? copypasted from lab 4
	
	//set Output Data Rate to 100Hz
	accWrite(0x2C, 0x0A); // 0x2C BW_RATE register, 0x0A 100Hz ODR
	
	//set full range mode
	accWrite(0x31, 0x08); // 0x31 DATA_FORMAT register, 0x08 Full resolution
	
	//enable measurement
	accWrite(0x2D, 0x08); // 0x2D POWER_CTL register, 0x08 Measurement enable	
}

void readValues(double* x, double* y, double* z){
	//TODO -> done? copypasted from lab 4
	
	// TODO DONE CHECK
	// find scaler from data sheet
	const double scaleFactor = 0.004;
	
	// read values into x,y,z using accRead
	int16_t xRaw = (accRead(0x33) << 8) | accRead(0x32); // X-Axis LSB first
	int16_t yRaw = (accRead(0x35) << 8) | accRead(0x34); // Y-Axis LSB first
	int16_t zRaw = (accRead(0x37) << 8) | accRead(0x36); // Z-Axis LSB first
	
	*x = (double)xRaw * scaleFactor;
	*y = (double)yRaw * scaleFactor;
	*z = (double)zRaw * scaleFactor;
}
