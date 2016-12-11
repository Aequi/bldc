/* ============================================================================================
* MPU6050_Config.h
*
*   This program is free software: you can redistribute it and/or modify
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
* Author:   Anh Vo Tuan
* Email:    votuananhs@gmail.com
* Brief:    The file is used to define parameters which use in the MPU6050 driver.
*           It will be compatible with ChibiOS.
*
*  ============================================================================================
*/

/* Includes */
#include "ch.h"
#include "hal.h"
#include "string.h"
#include "stdlib.h"
#include "MPU6050.h"
#include "MPU6050_Config.h"



/*=========================== DEFINE TYPE =======================================================*/
/*
 * 500KBaud, automatic wakeup, automatic recover
 * from abort mode.
 * See section 22.7.7 on the STM32 reference manual.
 */
static const I2CConfig i2cfg = {
    OPMODE_I2C,
    MPU6050_CLOCK_SPEED_I2C,
    STD_DUTY_CYCLE
};

/*=========================== DEFINE GLOBAL VARIABLE ============================================*/
static volatile uint8_t mpu6050_i2c_running = FALSE;
extern I2CDriver MPU6050_HW_I2C_DEV;

/*=========================== DEFINE PROTOTYPE FUNCTION =========================================*/
static void MPU6050_HwStartI2c(void);
static void MPU6050_HwStopI2c(void);
static void MPU6050_HwTryRestoreI2c(void);

static uint8_t MPU6050_Writes(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t number_bytes);
static uint8_t MPU6050_Reads(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t number_bytes);

static uint8_t MPU6050_Write(uint8_t slaveAddr, uint8_t regAddr, uint8_t data);
static uint8_t MPU6050_Read(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data);

static uint8_t MPU6050_WriteMask(uint8_t slaveAddr, uint8_t regAddr, uint8_t data, uint8_t mask, uint8_t offset);
static uint8_t MPU6050_ReadMask(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t mask, uint8_t offset);



/*=========================== FUNCTION ==========================================================*/
static void MPU6050_HwStartI2c(void) {
  i2cAcquireBus(&MPU6050_HW_I2C_DEV);

  if (!mpu6050_i2c_running) {
    palSetPadMode(MPU6050_HW_I2C_SCL_PORT, MPU6050_HW_I2C_SCL_PIN,
        PAL_MODE_ALTERNATE(MPU6050_HW_I2C_GPIO_AF) |
        PAL_STM32_OTYPE_OPENDRAIN |
        PAL_STM32_OSPEED_MID1 |
        PAL_STM32_PUDR_PULLUP);
    palSetPadMode(MPU6050_HW_I2C_SDA_PORT, MPU6050_HW_I2C_SDA_PIN,
        PAL_MODE_ALTERNATE(MPU6050_HW_I2C_GPIO_AF) |
        PAL_STM32_OTYPE_OPENDRAIN |
        PAL_STM32_OSPEED_MID1 |
        PAL_STM32_PUDR_PULLUP);

    i2cStart(&MPU6050_HW_I2C_DEV, &i2cfg);
    mpu6050_i2c_running = TRUE;
  }

  i2cReleaseBus(&MPU6050_HW_I2C_DEV);
}

static void MPU6050_HwStopI2c(void) {
  i2cAcquireBus(&MPU6050_HW_I2C_DEV);

  if (mpu6050_i2c_running) {
    palSetPadMode(MPU6050_HW_I2C_SCL_PORT, MPU6050_HW_I2C_SCL_PIN, PAL_MODE_INPUT);
    palSetPadMode(MPU6050_HW_I2C_SDA_PORT, MPU6050_HW_I2C_SDA_PIN, PAL_MODE_INPUT);

    i2cStop(&MPU6050_HW_I2C_DEV);
    mpu6050_i2c_running = FALSE;

  }

  i2cReleaseBus(&MPU6050_HW_I2C_DEV);
}


/**
 * Try to restore the i2c bus
 */
static void MPU6050_HwTryRestoreI2c(void) {
  if (mpu6050_i2c_running) {
    i2cAcquireBus(&MPU6050_HW_I2C_DEV);

    palSetPadMode(MPU6050_HW_I2C_SCL_PORT, MPU6050_HW_I2C_SCL_PIN,
        PAL_STM32_OTYPE_OPENDRAIN |
        PAL_STM32_OSPEED_MID1 |
        PAL_STM32_PUDR_PULLUP);

    palSetPadMode(MPU6050_HW_I2C_SDA_PORT, MPU6050_HW_I2C_SDA_PIN,
        PAL_STM32_OTYPE_OPENDRAIN |
        PAL_STM32_OSPEED_MID1 |
        PAL_STM32_PUDR_PULLUP);

    palSetPad(MPU6050_HW_I2C_SCL_PORT, MPU6050_HW_I2C_SCL_PIN);
    palSetPad(MPU6050_HW_I2C_SDA_PORT, MPU6050_HW_I2C_SDA_PIN);

    chThdSleep(1);

    for(int i = 0;i < 16;i++) {
      palClearPad(MPU6050_HW_I2C_SCL_PORT, MPU6050_HW_I2C_SCL_PIN);
      chThdSleep(1);
      palSetPad(MPU6050_HW_I2C_SCL_PORT, MPU6050_HW_I2C_SCL_PIN);
      chThdSleep(1);
    }

    // Generate start then stop condition
    palClearPad(MPU6050_HW_I2C_SDA_PORT, MPU6050_HW_I2C_SDA_PIN);
    chThdSleep(1);
    palClearPad(MPU6050_HW_I2C_SCL_PORT, MPU6050_HW_I2C_SCL_PIN);
    chThdSleep(1);
    palSetPad(MPU6050_HW_I2C_SCL_PORT, MPU6050_HW_I2C_SCL_PIN);
    chThdSleep(1);
    palSetPad(MPU6050_HW_I2C_SDA_PORT, MPU6050_HW_I2C_SDA_PIN);

    palSetPadMode(MPU6050_HW_I2C_SCL_PORT, MPU6050_HW_I2C_SCL_PIN,
        PAL_MODE_ALTERNATE(MPU6050_HW_I2C_GPIO_AF) |
        PAL_STM32_OTYPE_OPENDRAIN |
        PAL_STM32_OSPEED_MID1 |
        PAL_STM32_PUDR_PULLUP);

    palSetPadMode(MPU6050_HW_I2C_SDA_PORT, MPU6050_HW_I2C_SDA_PIN,
        PAL_MODE_ALTERNATE(MPU6050_HW_I2C_GPIO_AF) |
        PAL_STM32_OTYPE_OPENDRAIN |
        PAL_STM32_OSPEED_MID1 |
        PAL_STM32_PUDR_PULLUP);

    MPU6050_HW_I2C_DEV.state = I2C_STOP;
    i2cStart(&MPU6050_HW_I2C_DEV, &i2cfg);

    i2cReleaseBus(&MPU6050_HW_I2C_DEV);
  }
}



static uint8_t MPU6050_Writes(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t number_bytes)
{
  uint8_t rxbuf[1];
  uint8_t *txbuf = NULL;
  systime_t tmo = MS2ST(5);
  msg_t status = MSG_OK;
  uint8_t temp_result = TRUE;

  txbuf = (uint8_t*)calloc((number_bytes + 1), sizeof(uint8_t));
  txbuf[0] = regAddr;
  memcpy(&txbuf[1], data, number_bytes);

  i2cAcquireBus(&MPU6050_HW_I2C_DEV);
  status = i2cMasterTransmitTimeout(&MPU6050_HW_I2C_DEV, slaveAddr, txbuf, number_bytes + 1, rxbuf, 0, tmo);
  i2cReleaseBus(&MPU6050_HW_I2C_DEV);

  free(txbuf);

  if((MSG_RESET == status)||(MSG_TIMEOUT == status))
  {
    temp_result = FALSE;
  }

  return temp_result;
}


static uint8_t MPU6050_Write(uint8_t slaveAddr, uint8_t regAddr, uint8_t data)
{
  return MPU6050_Writes(slaveAddr, regAddr, &data, 1);
}


static uint8_t MPU6050_WriteMask(uint8_t slaveAddr, uint8_t regAddr, uint8_t data, uint8_t mask, uint8_t offset)
{
  uint8_t temp_data = 0;
  uint8_t temp_result = FALSE;

  temp_result = MPU6050_Reads(slaveAddr, regAddr, &temp_data, 1);
  if(TRUE == temp_result)
  {
    temp_data = (temp_data & (!mask)) | (data << offset);
    temp_result = MPU6050_Writes(slaveAddr, regAddr, &temp_data, 1);
  }
  return temp_result;
}


static uint8_t MPU6050_Reads(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t number_bytes)
{
  uint8_t txbuf[2] = {0, 0};
  systime_t tmo = MS2ST(5);
  msg_t status = MSG_OK;
  uint8_t temp_result = TRUE;

  txbuf[0] = regAddr;
  txbuf[1] = (slaveAddr << 1) | 0x01;

  i2cAcquireBus(&MPU6050_HW_I2C_DEV);
  status = i2cMasterTransmitTimeout(&MPU6050_HW_I2C_DEV, slaveAddr, txbuf, 2, data, number_bytes, tmo);
  i2cReleaseBus(&MPU6050_HW_I2C_DEV);

  if((MSG_RESET == status)||(MSG_TIMEOUT == status))
  {
    temp_result = FALSE;
  }

  return temp_result;
}

static uint8_t MPU6050_Read(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data)
{
  return MPU6050_Reads(slaveAddr, regAddr, data, 1);
}


static uint8_t MPU6050_ReadMask(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t mask, uint8_t offset)
{
  uint8_t temp_data = 0;
  uint8_t temp_result = FALSE;

  temp_result = MPU6050_Reads(slaveAddr, regAddr, &temp_data, 1);
  if(TRUE == temp_result)
  {
    temp_data = (temp_data & mask) >> offset;
  }
  *data = temp_data;
  return temp_result;
}

/** @defgroup MPU6050_Library
 * @{
 */

void MPU6050_Init(void) 
{
  /* 1. Initialize I2C peripheral in STM32f4 first */
  MPU6050_HwStartI2c();

  /* 2. Initialize MPU6050 */
  //reset the whole module first
  MPU6050_WriteMask(MPU6050_DEFAULT_ADDRESS, MPU6050_PWR_MGMT_1_ADDR, MPU6050_PWR_1_RESET_SET, MPU6050_PWR_1_RESET_MASK, MPU6050_PWR_1_RESET_OFFSET);

  chThdSleepMilliseconds(50); //wait for 50ms for the gyro to stable

  //PLL with Z axis gyroscope reference
  MPU6050_WriteMask(MPU6050_DEFAULT_ADDRESS, MPU6050_PWR_MGMT_1_ADDR, MPU6050_PWR_1_CLKSEL_PLL_ZGYRO, MPU6050_PWR_1_CLKSEL_MASK, MPU6050_PWR_1_CLKSEL_OFFSET);
	
  //DLPF_CFG = 3: Fs=1khz; bandwidth=42hz	
  MPU6050_WriteMask(MPU6050_DEFAULT_ADDRESS, MPU6050_CONFIG_ADDR, MPU6050_CONFIG_DLPF_CFG_3, MPU6050_CONFIG_DLPF_CFG_MASK, MPU6050_CONFIG_DLPF_CFG_OFFSET);
    	
  //500Hz sample rate ~ 2ms
  MPU6050_Write(MPU6050_DEFAULT_ADDRESS, MPU6050_SMPLRT_DIV_ADDR, 0x01);

  //Gyro full scale setting
  MPU6050_WriteMask(MPU6050_DEFAULT_ADDRESS, MPU6050_GYRO_CONFIG_ADDR, MPU6050_GCONFIG_FS_SEL_2000, MPU6050_GCONFIG_FS_SEL_MASK, MPU6050_GCONFIG_FS_SEL_OFFSET);
   
  //Accel full scale setting
  MPU6050_WriteMask(MPU6050_DEFAULT_ADDRESS, MPU6050_ACCEL_CONFIG_ADDR, MPU6050_ACONFIG_AFS_SEL_16g, MPU6050_ACONFIG_AFS_SEL_MASK, MPU6050_ACONFIG_AFS_SEL_OFFSET);

  //reset gyro and accel sensor
  MPU6050_Write(MPU6050_DEFAULT_ADDRESS, MPU6050_SIGNAL_PATH_RESET_ADDR, 0x07);
}


/*
*
*/
void MPU6050_DeInit(void)
{
    /* 1. MPU6050 */

    /* 2. I2C of STM32F4xx */
  MPU6050_HwStopI2c();
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, FALSE otherwise
 */
uint8_t MPU6050_TestConnection(void)
{
  return MPU6050_GetDeviceID() == 0x34 ? TRUE : FALSE; //0b110100; 8-bit representation in hex = 0x34
}
// WHO_AM_I register

/** Get Device ID.
 * This register is used to verify the identity of the device (0b110100).
 * @return Device ID (should be 0x34, 52 dec, 64 oct)
 * @see MPU6050_WHO_AM_I_ADDR
 * @see MPU6050_WHO_AM_I_MASK
 * @see MPU6050_WHO_AM_I_OFFSET
 */
uint8_t MPU6050_GetDeviceID(void)
{
  uint8_t tmp;
  MPU6050_ReadMask(MPU6050_DEFAULT_ADDRESS, MPU6050_WHO_AM_I_ADDR, &tmp, MPU6050_WHO_AM_I_MASK, MPU6050_WHO_AM_I_OFFSET);
  return tmp;
}

// GYRO_CONFIG register

/** Get full-scale gyroscope range.
 * The FS_SEL parameter allows setting the full-scale range of the gyro sensors,
 * as described in the table below.
 *
 * <pre>
 * 0 = +/- 250 degrees/sec
 * 1 = +/- 500 degrees/sec
 * 2 = +/- 1000 degrees/sec
 * 3 = +/- 2000 degrees/sec
 * </pre>
 *
 * @return Current full-scale gyroscope range setting
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_GYRO_CONFIG_ADDR
 * @see MPU6050_GCONFIG_FS_SEL_MASK
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
uint8_t MPU6050_GetFullScaleGyroRange(void)
{
  uint8_t tmp;
  MPU6050_ReadMask(MPU6050_DEFAULT_ADDRESS, MPU6050_GYRO_CONFIG_ADDR, &tmp, MPU6050_GCONFIG_FS_SEL_MASK, MPU6050_GCONFIG_FS_SEL_OFFSET);
  return tmp;
}

/** Get full-scale accelerometer range.
 * The FS_SEL parameter allows setting the full-scale range of the accelerometer
 * sensors, as described in the table below.
 *
 * <pre>
 * 0 = +/- 2g
 * 1 = +/- 4g
 * 2 = +/- 8g
 * 3 = +/- 16g
 * </pre>
 *
 * @return Current full-scale accelerometer range setting
 * @see MPU6050_ACCEL_FS_2
 * @see MPU6050_ACCEL_CONFIG_ADDR
 * @see MPU6050_ACONFIG_AFS_SEL_MASK
 * @see MPU6050_ACONFIG_AFS_SEL_OFFSET
 */
uint8_t MPU6050_GetFullScaleAccelRange(void)
{
  uint8_t tmp;
  MPU6050_ReadMask(MPU6050_DEFAULT_ADDRESS, MPU6050_ACCEL_CONFIG_ADDR, &tmp, MPU6050_ACONFIG_AFS_SEL_MASK, MPU6050_ACONFIG_AFS_SEL_OFFSET);
  return tmp;
}

/** Get sleep mode status.
 * Setting the SLEEP bit in the register puts the device into very low power
 * sleep mode. In this mode, only the serial interface and internal registers
 * remain active, allowing for a very low standby current. Clearing this bit
 * puts the device back into normal mode. To save power, the individual standby
 * selections for each of the gyros should be used if any gyro axis is not used
 * by the application.
 * @return Current sleep mode enabled status
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_SLEEP_BIT
 */
uint8_t MPU6050_GetSleepModeStatus(void)
{
  uint8_t tmp;
  MPU6050_ReadMask(MPU6050_DEFAULT_ADDRESS, MPU6050_PWR_MGMT_1_ADDR, &tmp, MPU6050_PWR_1_SLEEP_MASK, MPU6050_PWR_1_SLEEP_OFFSET);
  return tmp == 0x00 ? FALSE : TRUE;
}


/** Get raw 6-axis motion sensor readings (accel/gyro).
 * Retrieves all currently available motion sensor values.
 * @param AccelGyro 16-bit signed integer array of length 6
 * @see MPU6050_RA_ACCEL_XOUT_H
 */
void MPU6050_GetRawAccelGyro(int16_t *AccelGyro)
{
  uint8_t tmpBuffer[14];
  uint16_t tmpHigh = 0;
  uint16_t tmpLow = 0;
  uint8_t index_array;


  MPU6050_Writes(MPU6050_DEFAULT_ADDRESS, MPU6050_ACCEL_XOUT_H_ADDR, tmpBuffer, 14);

  /* Get acceleration */
  for (index_array = 0; index_array < 3; index_array++)
  {
      tmpHigh = (int16_t)(((uint16_t)tmpBuffer[index_array << 1] << (uint16_t)8 ) & 0xFF00);
      tmpLow = (int16_t)((uint16_t)tmpBuffer[(index_array << 1) | 0x0001] & 0x00FF);
      AccelGyro[index_array] = tmpHigh | tmpLow;
  }

  /* Get gyroscope */
  for (index_array = 4; index_array < 7; index_array++)
  {
      tmpHigh = (int16_t)(((uint16_t)tmpBuffer[index_array << 1] << (uint16_t)8 ) & 0xFF00);
      tmpLow = (int16_t)((uint16_t)tmpBuffer[(index_array << 1) | 0x0001] & 0x00FF);
      AccelGyro[index_array-1] = tmpHigh | tmpLow;
  }
}

/**
 * @}
 */
/***** END OF FILE ****/
