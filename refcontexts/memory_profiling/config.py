"""
config.py
************************

:module: config

:synopsis: This contains the configuration used by memory_analyser.py.

.. currentmodule:: config

:date: 25 May 2018

:Version: v0.0.1

:Release: 

:Copyright: (C) Bosch Connected Devices and Solutions GmbH.
    All Rights Reserved. Confidential.
    Distribution only to people who need to know this information in
    order to do their job.(Need-to-know principle).
    Distribution to persons outside the company, only if these persons
    signed a non-disclosure agreement.
    Electronic transmission, e.g. via electronic mail, must be made in
    encrypted form.

"""

import os

rootPath = os.getcwd()
dotAFilePath = rootPath + '\\release'  # path of the .a file

maxDetailLevel = 2  # Maximum supported detail levels

reportDirectory = 'memory_report'  # Directory to store the html reports
pageTitle = 'Memory Profiling'  # Page title of the html report
tableHeadingLevelOne = 'Summary Report(Size in Bytes)'  # Heading of the memory report table in level one
tableHeadingLevelTwo = 'Module Memory Report(Size in Bytes)'  # Heading of the memory report table in level two

firmwareRAMSectionList = ['.data', '.bss', '.stack']  # List of section to calculate RAM size
firmwareFLASHSectionList = ['.text', '.data', '.ro']  # List of section to calculate FLASH size

# list of section to calculate Other size
firmwareOtherSectionList = ['.rel', '.debug', '.comment', '.iar', '.ARM', '.shstrtab', 'strtab', 'symtab']

try:
    # assign the environmental variable if defined
    sevenZipExePath = os.path.normcase(os.environ['SEVENZIP_PATH']+'/7z.exe')
except:
    # assign the default environmental variable.
    sevenZipExePath = '7z.exe'
