/* ============================================================================================
* MPU6050_Config.h
*
*	This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*   ------------------------------------------------------------------------------
* Author: 	Anh Vo Tuan
* Email: 	votuananhs@gmail.com
* Brief:	The file is used to define parameters which use in the MPU6050 driver.
* 			It will be compatible with ChibiOS.
*
*  ============================================================================================
*/

#ifndef _MPU6050_CONFIG_H_
#define _MPU6050_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Select setting I2C channel */
#define	MPU6050_USING_I2C1 FALSE
#define	MPU6050_USING_I2C2 TRUE
#define	MPU6050_USING_I2C3 FALSE



/* I2C's baudrate (KB) */
#define MPU6050_CLOCK_SPEED_I2C			100000

// I2C Peripheral
/* The channel I2C of STM32F4xx will be connected with MPU6050 */
#if (MPU6050_USING_I2C1 == TRUE)
	#define MPU6050_HW_I2C_DEV			I2CD1
	#define MPU6050_HW_I2C_GPIO_AF		((uint8_t)0x04)
	#define MPU6050_HW_I2C_SCL_PORT		GPIOA
	#define MPU6050_HW_I2C_SCL_PIN		10
	#define MPU6050_HW_I2C_SDA_PORT		GPIOA
	#define MPU6050_HW_I2C_SDA_PIN		11

#elif (MPU6050_USING_I2C2 == TRUE)
	#define MPU6050_HW_I2C_DEV			I2CD2
	#define MPU6050_HW_I2C_GPIO_AF		((uint8_t)0x04)
	#define MPU6050_HW_I2C_SCL_PORT		GPIOB
	#define MPU6050_HW_I2C_SCL_PIN		10
	#define MPU6050_HW_I2C_SDA_PORT		GPIOB
	#define MPU6050_HW_I2C_SDA_PIN		11

#elif (MPU6050_USING_I2C3 == TRUE)
	#define MPU6050_HW_I2C_DEV			I2CD3
	#define MPU6050_HW_I2C_GPIO_AF		((uint8_t)0x04)
	#define MPU6050_HW_I2C_SCL_PORT		GPIOB
	#define MPU6050_HW_I2C_SCL_PIN		10
	#define MPU6050_HW_I2C_SDA_PORT		GPIOB
	#define MPU6050_HW_I2C_SDA_PIN		11
#else
	#error "Don't initialize a connection I2C bus !"
#endif


/* Number of getting sample to detect offset gyroscope */
#define NUMBER_GET_SAMPLE_OFFSET_GYRO	100

#ifdef __cplusplus
}
#endif

#endif /* _MPU6050_CONFIG_H_ */
/*****END OF FILE****/
