#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include "comm_usb_serial.h"
//#include "mpu6050.h"
#include "imu.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(4096)
#define TEST_WA_SIZE    THD_WORKING_AREA_SIZE(512)

void main_printf(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	chvprintf((BaseSequentialStream*)&SDU1, fmt, ap);
	va_end(ap);
}

static void cmd_val(BaseSequentialStream *chp, int argc, char *argv[]) {
  chprintf(chp, "Running test MPU6050 driver !\r\n");
}

static const ShellCommand commands[] = {
		{"help", cmd_val},
		{NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
		(BaseSequentialStream *)&SDU1,
		commands
};


/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetPad(GPIOD, GPIOD_LED3);       /* Orange.  */
    chThdSleepMilliseconds(500);
    palClearPad(GPIOD, GPIOD_LED3);     /* Orange.  */
    chThdSleepMilliseconds(500);
  }
}

/*
* read value from mpu6050 and print on terminal
*/
/*
static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Thread2, arg) {
  uint8_t temp_id_mpu6050;
  int16_t Accel_Gyro_Data[6];
  int16_t GyroPitch;
  float valueOffsetPitch;


  // initial mpu6050
  main_printf("Read value Accel and Gyro from MPU6050:\r\n");
  MPU6050_Init();
  chThdSleepMilliseconds(1000);

  temp_id_mpu6050 = MPU6050_TestConnection();
  //main_printf("ID MPU6050: %d\r\n",MPU6050_GetDeviceID());
  if (temp_id_mpu6050)
  {
    main_printf("Successful connection with the mpu6050 board\r\n");
  }
  else
  {
    main_printf("Connection is false !!!\r\n");
  }
  valueOffsetPitch = MPU6050_GetOffsetPitch();
  main_printf("Offset GyroPitch = %5.3f\r\n",valueOffsetPitch);
  for(;;) {
    //main_printf("ID MPU6050: %d\r\n",MPU6050_GetDeviceID());
    //MPU6050_GetRawAccelGyro(Accel_Gyro_Data);
    //main_printf("aX = %d\taY = %d\taZ = %d\r\n",Accel_Gyro_Data[0],Accel_Gyro_Data[1],Accel_Gyro_Data[2]);
    //main_printf("gX = %d\tgY = %d\tgZ = %d\r\n",Accel_Gyro_Data[3],Accel_Gyro_Data[4],Accel_Gyro_Data[5]);
    MPU6050_GetGyroPitch(&GyroPitch);
    main_printf("gY = %d\tangleY = %5.3f\r\n",GyroPitch,((float)GyroPitch - valueOffsetPitch)* 0.0181); //0.0595/3.28);
    chThdSleepMilliseconds(100);
  }
}
*/

int main(void) {
	//thread_t *shelltp = NULL;

	// For ChibiOS
	halInit();
	chSysInit();

	// For the usb-serial shell
	comm_usb_serial_init();
	//shellInit();

	chThdSleepMilliseconds(1000);
  Imu_Init();


  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  //chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO+1, Thread2, NULL);

	// Main loop where only the shell is handled
	for(;;) {
  /*
		if (!shelltp && (SDU1.config->usbp->state == USB_ACTIVE))
			shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
		else if (chThdTerminatedX(shelltp)) {
			chThdRelease(shelltp);    // Recovers memory of the previous shell.
			shelltp = NULL;           // Triggers spawning of a new shell.
		}
    */
		chThdSleepMilliseconds(1000);
	}
}
