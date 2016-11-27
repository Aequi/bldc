APPSRC =	applications/app.c \
			applications/app_ppm.c \
			applications/app_adc.c \
			applications/app_sten.c \
			applications/app_uartcomm.c \
			applications/app_nunchuk.c \
			applications/app_euc.c

APPINC = 	applications

# Add new files for the MPU6050 driver
APPSRC +=	applications/MPU6050/MPU6050.c

APPINC += 	applications/MPU6050
