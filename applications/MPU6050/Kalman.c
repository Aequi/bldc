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

#include "Kalman.h"


void Kalman_Init(KalmanFilterType *itemFilter)
{
    /* We will set the variables like so, these can also be tuned by the user */
    itemFilter->Q_angle = 0.001f;
    itemFilter->Q_bias = 0.003f;
    itemFilter->R_measure = 0.03f;

    itemFilter->angle = 0.0f; // Reset the angle
    itemFilter->bias = 0.0f; // Reset bias

    itemFilter->P[0][0] = 0.0f; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
    itemFilter->P[0][1] = 0.0f;
    itemFilter->P[1][0] = 0.0f;
    itemFilter->P[1][1] = 0.0f;
};

// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
float Kalman_GetAngle(KalmanFilterType *itemFilter, float newAngle, float newRate, float dt) {
    float S;
    float K[2]; // Kalman gain - This is a 2x1 vector
    float y;
    float P00_temp;
    float P01_temp;


    // KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
    // Modified by Kristian Lauszus
    // See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

    // Discrete Kalman filter time update equations - Time Update ("Predict")
    // Update xhat - Project the state ahead
    /* Step 1 */
    itemFilter->rate = newRate - itemFilter->bias;
    itemFilter->angle += dt * itemFilter->rate;

    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    itemFilter->P[0][0] += dt * (dt*itemFilter->P[1][1] - itemFilter->P[0][1] - itemFilter->P[1][0] + itemFilter->Q_angle);
    itemFilter->P[0][1] -= dt * itemFilter->P[1][1];
    itemFilter->P[1][0] -= dt * itemFilter->P[1][1];
    itemFilter->P[1][1] += itemFilter->Q_bias * dt;

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    S = itemFilter->P[0][0] + itemFilter->R_measure; // Estimate error
    /* Step 5 */
    K[0] = itemFilter->P[0][0] / S;
    K[1] = itemFilter->P[1][0] / S;

    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    /* Step 3 */
    y = newAngle - itemFilter->angle; // Angle difference
    /* Step 6 */
    itemFilter->angle += K[0] * y;
    itemFilter->bias += K[1] * y;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    P00_temp = itemFilter->P[0][0];
    P01_temp = itemFilter->P[0][1];

    itemFilter->P[0][0] -= K[0] * P00_temp;
    itemFilter->P[0][1] -= K[0] * P01_temp;
    itemFilter->P[1][0] -= K[1] * P00_temp;
    itemFilter->P[1][1] -= K[1] * P01_temp;

    return itemFilter->angle;
};

void Kalman_SetAngle(KalmanFilterType *itemFilter, float angle) { itemFilter->angle = angle; }; // Used to set angle, this should be set as the starting angle
float Kalman_GetRate(KalmanFilterType *itemFilter) { return itemFilter->rate; }; // Return the unbiased rate

/* These are used to tune the Kalman filter */
void Kalman_SetQangle(KalmanFilterType *itemFilter, float Q_angle) { itemFilter->Q_angle = Q_angle; };
void Kalman_SetQbias(KalmanFilterType *itemFilter, float Q_bias) { itemFilter->Q_bias = Q_bias; };
void Kalman_SetRmeasure(KalmanFilterType *itemFilter, float R_measure) { itemFilter->R_measure = R_measure; };

float Kalman_GetQangle(KalmanFilterType *itemFilter) { return itemFilter->Q_angle; };
float Kalman_GetQbias(KalmanFilterType *itemFilter) { return itemFilter->Q_bias; };
float Kalman_GetRmeasure(KalmanFilterType *itemFilter) { return itemFilter->R_measure; };

