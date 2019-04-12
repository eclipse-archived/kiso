BCDS_BSTLIB_PATH = 3rd-party/bstlib

BCDS_INCLUDES = \
	-I$(BCDS_BSTLIB_PATH)/BMA2x2_driver \
	-I$(BCDS_BSTLIB_PATH)/BME280_driver \
	-I$(BCDS_BSTLIB_PATH)/BMG160_driver \
	-I$(BCDS_BSTLIB_PATH)/BMI160_driver \
	-I$(BCDS_BSTLIB_PATH)/BMM050_driver \
	-I$(BCDS_BSTLIB_PATH)/BMP180_driver \
	-I$(BCDS_BSTLIB_PATH)/BMP280_driver \
	-I$(BCDS_BSTLIB_PATH)/BNO055_driver

# A variable which evaluates to a list of all source code files (*.c)
# of BSTLIB(BSD open source) 
BCDS_SOURCE_FILES = \
	$(BCDS_BSTLIB_PATH)/BMA2x2_driver/bma2x2.c \
	$(BCDS_BSTLIB_PATH)/BME280_driver/bme280.c \
	$(BCDS_BSTLIB_PATH)/BMG160_driver/bmg160.c \
	$(BCDS_BSTLIB_PATH)/BMI160_driver/bmi160.c \
	$(BCDS_BSTLIB_PATH)/BMM050_driver/bmm050.c \
	$(BCDS_BSTLIB_PATH)/BMP180_driver/bmp180.c \
	$(BCDS_BSTLIB_PATH)/BMP280_driver/bmp280.c \
	$(BCDS_BSTLIB_PATH)/BNO055_driver/bno055.c