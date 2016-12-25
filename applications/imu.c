/* ============================================================================================
* MPU6050.h
*
* This program is free software: you can redistribute it and/or modify
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
* Email:  votuananhs@gmail.com
* Brief:  The file is used to define parameters which use in the MPU6050 driver.
*       It will be compatible with ChibiOS.
*
*  ============================================================================================
*/

#include "ch.h"
#include "imu.h"
#include "Kalman.h"
#include "mpu6050.h"
#include <math.h>
#include <stdlib.h>

KalmanFilterType gyroPitchAxis;
KalmanFilterType gyroRollAxis;

static double accOffsetX, accOffsetY, accOffsetZ;
static double gyroOffsetX, gyroOffsetY, gyroOffsetZ;
static volatile double compAngleX, compAngleY; // Calculated angle using a complementary filter

/* task for processing data from MPU6050 */
static THD_WORKING_AREA(waProcessDataMpu6050, 128);
static THD_FUNCTION(ProcessDataMpu6050, arg);
/* task for controlling motor */
static THD_WORKING_AREA(waControlMotor, 128);
static THD_FUNCTION(ControlMotor, arg);


void Imu_Init(void)
{
  /* initialize mpu6050 driver */
  MPU6050_Init();
  /* waiting mpu6050 sensor is initialized successfully */
  chThdSleepMilliseconds(1000);
  /* initialize some parameters for Kalman filter that will be used for Pitch axis */
  Kalman_Init(&gyroPitchAxis);
  Kalman_Init(&gyroRollAxis);

  MPU6050_GetOffsetAxis(&accOffsetX, &accOffsetY, &accOffsetZ, &gyroOffsetX, &gyroOffsetY, &gyroOffsetZ);
  // main_printf("accOffsetX = %5.3f\r\n",accOffsetX);
  // main_printf("accOffsetY = %5.3f\r\n",accOffsetY);
  // main_printf("accOffsetZ = %5.3f\r\n",accOffsetZ);
  // main_printf("gyroOffsetX = %5.3f\r\n",gyroOffsetX);
  // main_printf("gyroOffsetY = %5.3f\r\n",gyroOffsetY);
  // main_printf("gyroOffsetZ = %5.3f\r\n",gyroOffsetZ);

  /* create a thread in ChibiOS to process data of mpu6050 */
  chThdCreateStatic(waProcessDataMpu6050, sizeof(waProcessDataMpu6050), NORMALPRIO+2, ProcessDataMpu6050, NULL);
  /* create a thread in ChibiOS to control motor by using PID method */
  chThdCreateStatic(waControlMotor, sizeof(waControlMotor), NORMALPRIO+1, ControlMotor, NULL);
}


static THD_FUNCTION(ProcessDataMpu6050, arg)
{
  chRegSetThreadName("IMU with mpu6050 sensor");

  int16_t Accel_Gyro_Data[6];
  static volatile double accX, accY, accZ;
  static volatile double gyroX, gyroY;
  static volatile double pitch, roll;
  static volatile systime_t timer, temp_timer;
  static volatile double gyroXangle, gyroYangle; // Angle calculate using the gyro only
  static volatile double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter
  static volatile double dt;
  static volatile double gyroXrate; // Convert to deg/s
  static volatile double gyroYrate; // Convert to deg/s

  MPU6050_GetRawAccelGyro(Accel_Gyro_Data);
  accX = (double)Accel_Gyro_Data[0] - accOffsetX;
  accY = (double)Accel_Gyro_Data[1] - accOffsetY;
  accZ = (double)Accel_Gyro_Data[2] - accOffsetZ;
  gyroX = (double)Accel_Gyro_Data[3] - gyroOffsetX;
  gyroY = (double)Accel_Gyro_Data[4] - gyroOffsetY;

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  roll  = atan2(accY, accZ) * RAD_TO_DEG;
  pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif
  Kalman_SetAngle(&gyroRollAxis, roll);
  Kalman_SetAngle(&gyroPitchAxis, pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;
  timer = chVTGetSystemTime();

  for(;;) {
    MPU6050_GetRawAccelGyro(Accel_Gyro_Data);
    temp_timer = chVTGetSystemTime();
    dt = ((double)(temp_timer - timer))/S2ST(1); // Calculate delta time
    timer = temp_timer;

    accX = (double)Accel_Gyro_Data[0] - accOffsetX;
    accY = (double)Accel_Gyro_Data[1] - accOffsetY;
    accZ = (double)Accel_Gyro_Data[2] - accOffsetZ;
    gyroX = (double)Accel_Gyro_Data[3] - gyroOffsetX;
    gyroY = (double)Accel_Gyro_Data[4] - gyroOffsetY;

    // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
    // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
    // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
    roll  = atan2(accY, accZ) * RAD_TO_DEG;
    pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
    roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif

    gyroXrate = gyroX / ((double)16.384); // Convert to deg/s -> FS_SEL = 3
    gyroYrate = gyroY / ((double)16.384); // Convert to deg/s -> FS_SEL = 3

#ifdef RESTRICT_PITCH
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90))
    {
      Kalman_SetAngle(&gyroRollAxis, roll);
      compAngleX = roll;
      kalAngleX = roll;
      gyroXangle = roll;
    } 
    else
    {
      kalAngleX = Kalman_GetAngle(&gyroRollAxis, roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
    }

    if (abs(kalAngleX) > 90) 
    {
      gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
    }
    kalAngleY = Kalman_GetAngle(&gyroPitchAxis, pitch, gyroYrate, dt);
#else
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) 
    {
      Kalman_SetAngle(&gyroPitchAxis, pitch);
      compAngleY = pitch;
      kalAngleY = pitch;
      gyroYangle = pitch;
    }
    else
    {
      kalAngleY = Kalman_GetAngle(&gyroPitchAxis, pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter
    }

    if (abs(kalAngleY) > 90)
    {
      gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
    }
    kalAngleX = Kalman_GetAngle(&gyroRollAxis, roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
#endif

    gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
    gyroYangle += gyroYrate * dt;

    // compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
    // compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

    compAngleX = ((double)0.90) * (compAngleX + gyroXrate * dt) + ((double)0.10) * roll; // Calculate the angle using a Complimentary filter
    compAngleY = ((double)0.90) * (compAngleY + gyroYrate * dt) + ((double)0.10) * pitch;

    // Reset the gyro angle when it has drifted too much
    if (gyroXangle < -180 || gyroXangle > 180)
      gyroXangle = kalAngleX;
    if (gyroYangle < -180 || gyroYangle > 180)
      gyroYangle = kalAngleY;

    // main_printf("%5.3f\t%5.3f\t%5.3f\t%5.3f\t\t",roll, gyroXangle, compAngleX, kalAngleX);
    // main_printf("%5.3f\t%5.3f\t%5.3f\t%5.3f\t\r\n",pitch, gyroYangle, compAngleY, kalAngleY);
    // main_printf("%5.3f\t%5.3f\t%5.3f\t%5.3f\t",pitch, gyroYangle, compAngleY, kalAngleY);
    // main_printf("%5.3f\t%5.3f\t\r\n",dt, timer);
    // main_printf("pitch=%5.3f\tgyroYangle=%5.3f\tcompAngleY=%5.3f\tkalAngleY=%5.3f\r\n",pitch, gyroYangle, compAngleY, kalAngleY);
    // temp = RAD_TO_DEG;
    // main_printf("%5.3f\r\n",temp);

    chThdSleepMilliseconds(9);
  }
}

/*
*   In the task, you can use angle parameter such as: "compAngleX" or "compAngleY" to control motor.
*   They will be sampled in around 10 ms
*/
static THD_FUNCTION(ControlMotor, arg)
{
  chRegSetThreadName("Control motor by PID method");

  /* local variable for this task */

  /* main code for task */
  for(;;) {
    /* for your code */


    /* You need to calculate execution time and then add delay time for this task */
    chThdSleepMilliseconds(9);
  }
}

/***** END OF FILE ****/
