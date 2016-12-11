The testing mpu6050 driver is using STM32F4 discovery board.

Build testing project for mpu6050 by following these steps below:

- 1. jump into the test folder "cd ./test"
- 2. type "make clean" to clean everything in build folder
- 3. type "make" to build test project for stm32f4 discovery board
- 4. type "make upload" to write binary file (.elf) into stm32f407 silicon


Connection with GY-521 board that is using MPU6050 chip

GY-521		STM32F4 discovery
VCC 	-> 	+5V
GND 	-> 	GND
SCL		->	PB.10
SDA		->	PB.11
XDA		->	
XCL		->	
AD0		->	
INT		->	

