/* ============================================================================================
* MPU6050.h
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

#ifndef _MPU6050_H_
#define _MPU6050_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "datatypes.h"

#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     (MPU6050_ADDRESS_AD0_LOW<<1)

/* SELF_TEST_X */
#define MPU6050_SELF_TEST_X_ADDR		0x0D
#define MPU6050_SELF_TEST_XA_MASK		0xE0
#define MPU6050_SELF_TEST_XA_OFFSETT	5
#define MPU6050_SELF_TEST_XG_MASK		0x1F
#define MPU6050_SELF_TEST_XG_OFFSETT	0

/* SELF_TEST_Y */
#define MPU6050_SELF_TEST_Y_ADDR		0x0E
#define MPU6050_SELF_TEST_YA_MASK		0xE0
#define MPU6050_SELF_TEST_YA_OFFSETT	5
#define MPU6050_SELF_TEST_YG_MASK		0x1F
#define MPU6050_SELF_TEST_YG_OFFSETT	0

/* SELF_TEST_Z */
#define MPU6050_SELF_TEST_Z_ADDR		0x0F
#define MPU6050_SELF_TEST_ZA_MASK		0xE0
#define MPU6050_SELF_TEST_ZA_OFFSETT	5
#define MPU6050_SELF_TEST_ZG_MASK		0x1F
#define MPU6050_SELF_TEST_ZG_OFFSETT	0

/* SELF_TEST_A */
#define MPU6050_SELF_TEST_A_ADDR		0x10
#define MPU6050_SELF_TEST_A_XA_MASK		0x30
#define MPU6050_SELF_TEST_A_XA_OFFSETT	4
#define MPU6050_SELF_TEST_A_YA_MASK		0x0C
#define MPU6050_SELF_TEST_A_YA_OFFSETT	2
#define MPU6050_SELF_TEST_A_ZA_MASK		0x03
#define MPU6050_SELF_TEST_A_ZA_OFFSETT	0

/* SMPLRT_DIV */
#define MPU6050_SMPLRT_DIV_ADDR			0x19

/* CONFIG */
#define MPU6050_CONFIG_ADDR				0x1A
#define MPU6050_CONFIG_EXT_SYNC_SET_MASK	0x38
#define MPU6050_CONFIG_EXT_SYNC_SET_OFFSET	3
#define MPU6050_CONFIG_DLPF_CFG_MASK	0x38
#define MPU6050_CONFIG_DLPF_CFG_OFFSET	0

#define MPU6050_CONFIG_DLPF_CFG_0		0
#define MPU6050_CONFIG_DLPF_CFG_1		1
#define MPU6050_CONFIG_DLPF_CFG_2		2
#define MPU6050_CONFIG_DLPF_CFG_3		3
#define MPU6050_CONFIG_DLPF_CFG_4		4
#define MPU6050_CONFIG_DLPF_CFG_5		5
#define MPU6050_CONFIG_DLPF_CFG_6		6
#define MPU6050_CONFIG_DLPF_CFG_7		7

/* GYRO_CONFIG */
#define MPU6050_GYRO_CONFIG_ADDR		0x1B
#define MPU6050_ACONFIG_XG_ST_MASK		0x80
#define MPU6050_ACONFIG_XG_ST_OFFSET	7
#define MPU6050_ACONFIG_YG_ST_MASK		0x40
#define MPU6050_ACONFIG_YG_ST_OFFSET	6
#define MPU6050_ACONFIG_ZG_ST_MASK		0x20
#define MPU6050_ACONFIG_ZG_ST_OFFSET	5
#define MPU6050_GCONFIG_FS_SEL_MASK		0x18
#define MPU6050_GCONFIG_FS_SEL_OFFSET	3

#define MPU6050_GCONFIG_FS_SEL_250		0
#define MPU6050_GCONFIG_FS_SEL_500		1
#define MPU6050_GCONFIG_FS_SEL_1000		2
#define MPU6050_GCONFIG_FS_SEL_2000		3

/* ACCEL_CONFIG */
#define MPU6050_ACCEL_CONFIG_ADDR		0x1C
#define MPU6050_ACONFIG_XA_ST_MASK		0x80
#define MPU6050_ACONFIG_XA_ST_OFFSET	7
#define MPU6050_ACONFIG_YA_ST_MASK		0x40
#define MPU6050_ACONFIG_YA_ST_OFFSET	6
#define MPU6050_ACONFIG_ZA_ST_MASK		0x20
#define MPU6050_ACONFIG_ZA_ST_OFFSET	5
#define MPU6050_ACONFIG_AFS_SEL_MASK	0x18
#define MPU6050_ACONFIG_AFS_SEL_OFFSET	3

#define MPU6050_ACONFIG_AFS_SEL_2g		0
#define MPU6050_ACONFIG_AFS_SEL_4g		1
#define MPU6050_ACONFIG_AFS_SEL_8g		2
#define MPU6050_ACONFIG_AFS_SEL_16g		3

/* ACCEL_XOUT_H */
#define MPU6050_ACCEL_XOUT_H_ADDR		0x3B

/* TEMP_OUT_H */
#define MPU6050_TEMP_OUT_H_ADDR			0x41

/* GYRO_XOUT_H */
#define MPU6050_GYRO_XOUT_H_ADDR		0x43

/* SIGNAL_PATH_RESET */
#define MPU6050_SIGNAL_PATH_RESET_ADDR	0x68
#define MPU6050_SIGNAL_GYRO_RESET_MASK		0x04
#define MPU6050_SIGNAL_GYRO_RESET_OFFSET	2
#define MPU6050_SIGNAL_ACCEL_RESET_MASK		0x02
#define MPU6050_SIGNAL_ACCEL_RESET_OFFSET	1
#define MPU6050_SIGNAL_TEMP_RESET_MASK		0x01
#define MPU6050_SIGNAL_TEMP_RESET_OFFSET	0

#define MPU6050_SIGNAL_GYRO_RESET_SET	0x01
#define MPU6050_SIGNAL_ACCEL_RESET_SET	0x01
#define MPU6050_SIGNAL_TEMP_RESET_SET	0x01


/* PWR_MGMT_1 */
#define MPU6050_PWR_MGMT_1_ADDR			0x6B
#define MPU6050_PWR_1_RESET_MASK		0x80
#define MPU6050_PWR_1_RESET_OFFSET		7
#define MPU6050_PWR_1_SLEEP_MASK		0x40
#define MPU6050_PWR_1_SLEEP_OFFSET		6
#define MPU6050_PWR_1_CYCLE_MASK		0x20
#define MPU6050_PWR_1_CYCLE_OFFSET		5
#define MPU6050_PWR_1_TEMP_DIS_MASK		0x08
#define MPU6050_PWR_1_TEMP_DIS_OFFSET	3
#define MPU6050_PWR_1_CLKSEL_MASK		0x07
#define MPU6050_PWR_1_CLKSEL_OFFSET		0

#define MPU6050_PWR_1_RESET_SET			1
#define MPU6050_PWR_1_CLKSEL_INTERNAL	0
#define MPU6050_PWR_1_CLKSEL_PLL_XGYRO	1
#define MPU6050_PWR_1_CLKSEL_PLL_YGYRO	2
#define MPU6050_PWR_1_CLKSEL_PLL_ZGYRO	3
#define MPU6050_PWR_1_CLKSEL_PLL_Ext32K	4
#define MPU6050_PWR_1_CLKSEL_PLL_Ext19M	5
#define MPU6050_PWR_1_CLKSEL_STOP_CLK	7

/* WHO_AM_I */
#define MPU6050_WHO_AM_I_ADDR			0x75
#define MPU6050_WHO_AM_I_MASK			0x7E
#define MPU6050_WHO_AM_I_OFFSET			0x1


void MPU6050_Init(void);

uint8_t MPU6050_TestConnection(void);

// GYRO_CONFIG register
uint8_t MPU6050_GetFullScaleGyroRange(void);
// ACCEL_CONFIG register
uint8_t MPU6050_GetFullScaleAccelRange(void);
// PWR_MGMT_1 register
uint8_t MPU6050_GetSleepModeStatus(void);
// WHO_AM_I register
uint8_t MPU6050_GetDeviceID(void);

void MPU6050_GetRawAccelGyro(uint16_t *AccelGyro);

void MPU6050_DeInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __MPU6050_H */
/***** END OF FILE ****/
