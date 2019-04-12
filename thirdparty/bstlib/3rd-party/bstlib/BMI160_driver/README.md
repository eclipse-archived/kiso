
CONTENTS OF THIS FILE
=======================
	* Introduction
	* Version
	* Integration details
	* Driver files information
	* Supported sensor interface
	* Copy right


INTRODUCTION
===============
	- This package contains the Bosch Sensortec MEMS BMI160 sensor driver (sensor API)
	- The sensor driver package includes bmi160.h, bmi160.c, bmi160_support.h and bmi160_support.c files

VERSION
=========
	- Version of bmi160 sensor driver is:
		* bmi160.c 		- V2.0.7
		* bmi160.h 		- V2.0.7
		* bmi160_support.c 	- V1.0.7
		* bmi160_support.h 	- V1.0.7

INTEGRATION DETAILS
=====================
	- Integrate bmi160.h and bmi160.c file in to your project.
	- The bmi160_support.c and bmi160_support.h file contains only examples for API use cases, so it is not required to integrate into project.

DRIVER FILES INFORMATION
===========================
	bmi160.h
	-----------
		* This header file has the register address definition, constant definitions, data type definition and supported sensor driver calls declarations.

	 bmi160.c
	------------
		* This file contains the implementation for the sensor driver APIs.

	 bmi160_support.c
	----------------------
		* This file shall be used as an user guidance, here you can find samples of
    			* Initialize the sensor with I2C/SPI communication
        				- Add your code to the SPI and/or I2C bus read and bus write functions.
            					- Return value can be chosen by yourself
           					- API just passes that value to your application code
        				- Add your code to the delay function
        				- Change I2C address accordingly in bmi160.h
   			* Different running modes(use cases) of BMI160 
			* Interrupt configuration

SUPPORTED SENSOR INTERFACE
====================================
	- This BMI160 sensor driver supports SPI and I2C interfaces


COPYRIGHT
===========
	- Copyright (C) 2015 - 2016 Bosch Sensortec GmbH

