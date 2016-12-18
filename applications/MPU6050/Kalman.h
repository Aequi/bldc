/* Copyright (C) 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 ----------------------------------------------
 votuananhs@gmail.com have updated source code to support C language
 */

#ifndef _Kalman_h_
#define _Kalman_h_

typedef struct __KalmanFilterType
{
    /* Kalman filter variables */
    float Q_angle; // Process noise variance for the accelerometer
    float Q_bias; // Process noise variance for the gyro bias
    float R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

    float angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
    float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
    float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

    float P[2][2]; // Error covariance matrix - This is a 2x2 matrix
} KalmanFilterType;

void Kalman_Init(KalmanFilterType *itemFilter);

// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
float Kalman_GetAngle(KalmanFilterType *itemFilter, float newAngle, float newRate, float dt);

void Kalman_SetAngle(KalmanFilterType *itemFilter, float angle); // Used to set angle, this should be set as the starting angle
float Kalman_GetRate(KalmanFilterType *itemFilter); // Return the unbiased rate

/* These are used to tune the Kalman filter */
void Kalman_SetQangle(KalmanFilterType *itemFilter, float Q_angle);
void Kalman_SetQbias(KalmanFilterType *itemFilter, float Q_bias);
void Kalman_SetRmeasure(KalmanFilterType *itemFilter, float R_measure);

float Kalman_GetQangle(KalmanFilterType *itemFilter);
float Kalman_GetQbias(KalmanFilterType *itemFilter);
float Kalman_GetRmeasure(KalmanFilterType *itemFilter);

#endif
