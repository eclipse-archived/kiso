
CONTENTS OF THIS FILE
=======================
	* Introduction
	* Version
	* Integration details
	* Driver files information
	* Supported sensor interface
	* Copyright


INTRODUCTION
===============
	- This package contains the Bosch Sensortec MEMS pressure sensor driver (sensor API)
	- The sensor driver package includes bmp280.h, bmp280.c and bmp280_support.c files

VERSION
=========
	- Version of bmp280 sensor driver is:
		* bmp280.c 		- V2.0.4
		* bmp280.h 		- V2.0.4
		* bmp280_support.c 	- V1.0.5

INTEGRATION DETAILS
=====================
	- Integrate bmp280.h and bmp280.c file in to your project.
	- The bmp280_support.c file contains only examples for API use cases, so it is not required to integrate into project.

DRIVER FILES INFORMATION
===========================
	bmp280.h
	-----------
		* This header file has the register address definition, constant definitions, data type definition and supported sensor driver calls declarations.

	 bmp280.c
	------------
		* This file contains the implementation for the sensor driver APIs.

	 bmp280_support.c
	----------------------
		* This file shall be used as an user guidance, here you can find samples of
    			* Initialize the sensor with I2C/SPI communication
        				- Add your code to the SPI and/or I2C bus read and bus write functions.
            					- Return value can be chosen by yourself
           					- API just passes that value to your application code
        				- Add your code to the delay function
        				- Change I2C address accordingly in bmp280.h
   			* Power mode configuration of the sensor
   			* Get and set functions usage
			* Reading the sensor read out data

SUPPORTED SENSOR INTERFACE
====================================
	- This pressure sensor driver supports I2C and SPI interfaces


COPYRIGHT
===========
	- Copyright (C) 2012 - 2015 Bosch Sensortec GmbH


