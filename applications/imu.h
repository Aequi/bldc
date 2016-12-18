/* ============================================================================================
* MPU6050.h
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

#ifndef _IMU_H_
#define _IMU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Kalman.h"

#define PI 3.1415926535897932384626433832795
#define RAD_TO_DEG 57.295779513082320876798154814105

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf


extern KalmanFilterType gyroPitchAxis;
extern KalmanFilterType gyroRollAxis;

extern void main_printf(const char *fmt, ...);

void Imu_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* _IMU_H_ */
/***** END OF FILE ****/
