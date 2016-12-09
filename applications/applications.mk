APPSRC =	applications/app.c \
			applications/app_ppm.c \
			applications/app_adc.c \
			applications/app_sten.c \
			applications/app_uartcomm.c \
			applications/app_nunchuk.c \
			applications/app_euc.c

APPINC = 	applications

# Add new files for the MPU6050 driver
include applications/MPU6050/mpu6050.mk

APPSRC +=	$(MPU6050SRC)

APPINC += 	$(MPU6050INC)
