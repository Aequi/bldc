/* ============================================================================================
* MPU6050_Config.h
* Author: 	Anh Vo Tuan
* Email: 	votuananhs@gmailc.om
* Brief:	The file is used to define parameters which use in the MPU6050 driver.
* 			It will be compatible with ChibiOS.
*   ------------------------------------------------------------------------------
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
* 
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
* 
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*  ============================================================================================
*/

#ifndef _MPU6050_CONFIG_H_
#define _MPU6050_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


/* I2C's baudrate (KB) */
#define MPU6050_CLOCK_SPEED_I2C			250000

// I2C Peripheral
/* The channel I2C of STM32F4xx will be connected with MPU6050 */
#define MPU6050_HW_I2C_DEV			I2CD1
#define MPU6050_HW_I2C_GPIO_AF		GPIO_AF_I2C2
#define MPU6050_HW_I2C_SCL_PORT		GPIOB
#define MPU6050_HW_I2C_SCL_PIN		10
#define MPU6050_HW_I2C_SDA_PORT		GPIOB
#define MPU6050_HW_I2C_SDA_PIN		11

#ifdef __cplusplus
}
#endif

#endif /* _MPU6050_CONFIG_H_ */
/*****END OF FILE****/
