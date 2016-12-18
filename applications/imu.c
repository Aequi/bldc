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

KalmanFilterType gyroPitchAxis;
KalmanFilterType gyroRollAxis;

static THD_WORKING_AREA(waProcessDataMpu6050, 128);
static THD_FUNCTION(ProcessDataMpu6050, arg);


void Imu_Init(void)
{
  /* initialize mpu6050 driver */
  MPU6050_Init();
  /* waiting mpu6050 sensor is initialized successfully */
  chThdSleepMilliseconds(1000);
  /* initialize some parameters for Kalman filter that will be used for Pitch axis */
  Kalman_Init(&gyroPitchAxis);
  Kalman_Init(&gyroRollAxis);

  /* create a thread in ChibiOS to process data of mpu6050 */
  chThdCreateStatic(waProcessDataMpu6050, sizeof(waProcessDataMpu6050), NORMALPRIO+1, ProcessDataMpu6050, NULL);
}


static THD_FUNCTION(ProcessDataMpu6050, arg)
{
  chRegSetThreadName("IMU with mpu6050 sensor");

  int16_t Accel_Gyro_Data[6];
  double accX, accY, accZ;
  double gyroX, gyroY, gyroZ;
  double pitch, roll;
  uint32_t timer, temp_timer;
  double gyroXangle, gyroYangle; // Angle calculate using the gyro only
  double compAngleX, compAngleY; // Calculated angle using a complementary filter
  double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter
  double dt;
  double gyroXrate; // Convert to deg/s
  double gyroYrate; // Convert to deg/s

  MPU6050_GetRawAccelGyro(Accel_Gyro_Data);
  accX = (double)Accel_Gyro_Data[0];
  accY = (double)Accel_Gyro_Data[1];
  accZ = (double)Accel_Gyro_Data[2];
  gyroX = (double)Accel_Gyro_Data[3];
  gyroY = (double)Accel_Gyro_Data[4];
  gyroZ = (double)Accel_Gyro_Data[5];

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
  timer = (uint32_t)ST2S(chVTGetSystemTime());

  for(;;) {
    MPU6050_GetRawAccelGyro(Accel_Gyro_Data);
    temp_timer = (uint32_t)ST2S(chVTGetSystemTime());
    dt = ((double)(temp_timer - timer))/1000000; // Calculate delta time
    timer = temp_timer;
    accX = (double)Accel_Gyro_Data[0];
    accY = (double)Accel_Gyro_Data[1];
    accZ = (double)Accel_Gyro_Data[2];
    gyroX = (double)Accel_Gyro_Data[3];
    gyroY = (double)Accel_Gyro_Data[4];
    gyroZ = (double)Accel_Gyro_Data[5];

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

    gyroXrate = gyroX / 16.384; // Convert to deg/s
    gyroYrate = gyroY / 16.384; // Convert to deg/s

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
    //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
    //gyroYangle += kalmanY.getRate() * dt;

    compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
    compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

    // Reset the gyro angle when it has drifted too much
    if (gyroXangle < -180 || gyroXangle > 180)
      gyroXangle = kalAngleX;
    if (gyroYangle < -180 || gyroYangle > 180)
      gyroYangle = kalAngleY;

    main_printf("%5.3f\t%5.3f\t%5.3f\t%5.3f\t\t",roll, gyroXangle, compAngleX, kalAngleX);
    main_printf("%5.3f\t%5.3f\t%5.3f\t%5.3f\t\r\n",pitch, gyroYangle, compAngleY, kalAngleY);
    //main_printf("pitch=%5.3f\tgyroYangle=%5.3f\tcompAngleY=%5.3f\tkalAngleY=%5.3f\r\n",pitch, gyroYangle, compAngleY, kalAngleY);

    chThdSleepMilliseconds(500);
  }
}

/***** END OF FILE ****/
