# garage-door

Overview
STM32-based garage door controller that:
- Uses I2C temperature sensor for auto open/close
- Accepts commands via UART or Bluetooth
- Uses SPI accelerometer to verify door orientation
- Non-blocking control with SysTick and DMA UART

Features
- Auto open > 28 °C, auto close < 26 °C
- Manual commands: O=Open, C=Close, S=Stop
- Temperature/accel checks to prevent over-driving
- UART + Bluetooth control at 9600 baud

Hardware (STM32L476)
- UART2 (PC): PA2/PA3
- UART1 (BT): PB6/PB7
- I2C1 (Temp): PB8/PB9
- SPI1 (Accel): PA4, PB3, PB4, PB5
- LED: PA5
- DMA: UART2_TX → Ch7, UART1_TX → Ch4

Build & Run
- Configure project at 80 MHz
- Include: SysClock.c, SysTimer.c, UART.c, DMA.c, I2C.c, SPI.c, accelerometer.c, LED.c, motor.*, main.c
- Wire per pinout, flash, open UART2 at 9600 baud

Operation
- Auto mode: Opens/closes based on temperature
- Manual mode: Send O, C, S via UART2 or Bluetooth
- Prints telemetry and status to UART
