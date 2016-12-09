#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include "comm_usb_serial.h"

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
    main_printf("Turn on led3\r\n");
    chThdSleepMilliseconds(500);
    palClearPad(GPIOD, GPIOD_LED3);     /* Orange.  */
    main_printf("Turn off led3\r\n");
    chThdSleepMilliseconds(500);
  }
}

int main(void) {
	thread_t *shelltp = NULL;

	// For ChibiOS
	halInit();
	chSysInit();

	// For the usb-serial shell
	comm_usb_serial_init();
	//shellInit();

	chThdSleepMilliseconds(1000);
  main_printf("AnhVT6\r\n");
  chThdSleepMilliseconds(2000);

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

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
