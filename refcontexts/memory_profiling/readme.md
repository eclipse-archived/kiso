
# Memory Profiling

----------
## Copyright Notice

Copyright (C) Bosch Connected Devices and Solutions GmbH.
All Rights Reserved. Confidential.
 
Distribution only to people who need to know this information in
order to do their job.(Need-to-know principle).
Distribution to persons outside the company, only if these persons 
signed a non-disclosure agreement.
Electronic transmission, e.g. via electronic mail, must be made in encrypted form.

----------

## Description

This module is used for parsing and extracting the size information for software components from a .a file.


**Features**

* It supports all shared packages.
* It provides two types of output
	* Table printed in console output.
	* And a HTML file.
* The output table generated depends upon the input level as shown below. 

**Level 1**

| Package        | FLASH     | RAM   |
| -------------- |:---------:| -----:|
| Algoxx_xx.a    | 156544     | 15445  |

**Level 2**

| Module Name    | FLASH | RAM |
| -------------- |:---------:| -----:|
| Algocompxxxx.o | 11654 | 1545 |
| Algocxxxx.o    | 11254 | 1045 |
| ...            | ...   | ...  |
| ...            | ...   | ...  |
| Algolastxxxx.o | 10054 | 145  |

* For level 1 and level 2 all sizes are in bytes
* The final outputs are sorted based on the alphabetical order of the modules for Level 2.
* The output level required can be specified as a command line argument (By default, the least detailed view is Level 1).

For more info please refer the design document - https://bcds02.de.bosch.com/confluence/display/CDDK/Memory+Profiling


### Tools Used

7zip tool is used to extract the data from the .a file.

**Steps**
* The scripts extract the list of .o files inside the .a file using 7zip tool and stores the .o file names in a list.
* Then the script extract the .a file to a folder with the same name as of .a file with the help of 7zip tool.
* Next the script parse the meta data of sections inside the .o file to calculate the RAM and FLASH which is extracted using 7zip tool.
* The above steps is repeated for all .o files and total RAM and FLASH are calculated module wise as well as overa ll size.
* For parsing and categorizing the script it uses regular expressions.


### Module used

#### html_generator.py

**Features**
* This module generates HTML file with tables.
* It also adds internal CSS, which will be used for viewing the HTML page locally.
* It also includes Jenkins style sheets which will be used to view the HTML page in the Jenkins job using HTML report publisher plugin.
* The name of HTML file to be generated can be passed as an argument.


### Syntax

memory_analyser.py output_file input_level

output_file : The output html file name with .html extension (File name with path is required)
input_level : The level of detail for the output tables. Currently supports 1 and 2.


**Examples**

memory_analyser.py Level1.html 1
memory_analyser.py Level1.html 2


    NOTE: The script also calculates the size of other section but it is only processed but not displayed. These details can be used in future for calculating further levels.

