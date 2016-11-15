/*
	Copyright 2016 Casainho	casainho@gmail.com

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * app_euc.c
 *
 *  Created on: 18 Nov 2016
 *      Author: casainho
 */

#include "app.h"

/*
#include "stm32f10x.h"
#include "imu.h"
#include "math.h"
#include "MPU6050/MPU6050.h"
#include "tinystdio/tinystdio.h"
#include "motor.h"

// X axis gives the front/rear inclination of the wheel
// Z axis gives the lateral inclination of the wheel

static s16 accel_gyro[6];
static float accel_gyro_average[6];

BOOL IMU_init(void)
{
  unsigned int i;

  MPU6050_I2C_Init();
  MPU6050_Initialize();

  // if the MPU6050 is ready, make "calibration"
  // read the sensor values and average
  if (MPU6050_TestConnection())
  {
    accel_gyro_average[0] = 0;
    accel_gyro_average[1] = 0;
    accel_gyro_average[2] = 0;
    accel_gyro_average[3] = 0;
    accel_gyro_average[4] = 0;
    accel_gyro_average[5] = 0;

    for (i = 0; i <= 10; i++)
    {
      MPU6050_GetRawAccelGyro (accel_gyro);

      accel_gyro_average[0] += accel_gyro[0];
      accel_gyro_average[1] += accel_gyro[1];
      accel_gyro_average[2] += accel_gyro[2];
      accel_gyro_average[3] += accel_gyro[3];
      accel_gyro_average[4] += accel_gyro[4];
      accel_gyro_average[5] += accel_gyro[5];

      delay_ms(50); //wait for 50ms for the gyro to stable
    }

    accel_gyro_average[0] /= 10;
    accel_gyro_average[1] /= 10;
    accel_gyro_average[2] /= 10;
    accel_gyro_average[3] /= 10;
    accel_gyro_average[4] /= 10;
    accel_gyro_average[5] /= 10;

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

*/

void app_euc_start (void)
{
  // TODO
}
