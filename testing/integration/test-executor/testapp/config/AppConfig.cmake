cmake_minimum_required(VERSION 3.6)

set(APP_CMDPROCESSOR_MAIN_PRIORITY 1 CACHE STRING "Main CmdProcessor task priority")
set(APP_CMDPROCESSOR_MAIN_STACKSIZE 1024 CACHE STRING "Main CmdProcessor stack-size")
set(APP_CMDPROCESSOR_MAIN_QLEN 10 CACHE STRING "Main CmdProcessor queue length")

set(APP_TASK_APP_PRIORITY 2 CACHE STRING "App task priority")
set(APP_TASK_APP_STACKSIZE 1024 CACHE STRING "App stack-size")

configure_file(${APP_CONFIG_DIR}/AppConfig.h.in ${CURRENT_CONFIG_DIR}/AppConfig.h)