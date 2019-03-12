.common_settings.mk
"BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\xdctools_3_50_03_33_core\xs.exe" \
--xdcpath="BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\kernel\tirtos\packages;BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source;BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack" \
iar.tools.configuro \
-c "C:\IARSystems\V_8_11_2\Embedded Workbench 8.0\arm" \
--cc "C:\IARSystems\V_8_11_2\Embedded Workbench 8.0\arm\bin\iccarm.exe" \
--device "CC2640R2F" \
--compileOptions \
"-D CC2640R2_LAUNCHXL -D CC26XX -D CC26XX_R2 -D DeviceFamily_CC26X0R2 -D ICALL_EVENTS \
-D ICALL_JT -D ICALL_LITE -D ICALL_MAX_NUM_ENTITIES=6 -D ICALL_MAX_NUM_TASKS=3 -D MAX_NUM_BLE_CONNS=1 -D POWER_SAVING -D RF_SINGLEMODE -D USE_ICALL -D xdc_runtime_Assert_DISABLE_ALL -D xdc_runtime_Log_DISABLE_ALL \
--diag_suppress Pa050 -o BCDS_TEST_APP_SOURCE_PATH\TestApp\debug\objects\ -DNDEBUG --endian=little --cpu=Cortex-M3 \
-f BCDS_TEST_APP_SOURCE_PATH\TestApp\buildComponents.opt \
-f BCDS_TEST_APP_SOURCE_PATH\config\stack\factoryConfig.opt \
-f BCDS_TEST_APP_SOURCE_PATH\config\stack\buildConfig.opt \
-f BCDS_TEST_APP_SOURCE_PATH\config\configPkg\compiler.opt.defs \
-f BCDS_TEST_APP_SOURCE_PATH\TestApp\IAR-Boundary.bdef \
-e --fpu=None \
--dlib_config \"C:\IARSystems\V_8_11_2\Embedded Workbench 8.0\arm\INC\c\DLib_Config_Normal.h\" \
-I BCDS_TEST_APP_SOURCE_PATH\config\ \
-I BCDS_TEST_APP_SOURCE_PATH\TestApp\include\ \
-I BCDS_TEST_APP_SOURCE_PATH\TestApp\source\Application\ \
-I BCDS_TEST_APP_SOURCE_PATH\TestApp\source\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\controller\cc26xx_r2\inc\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\inc\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\common\cc26xx\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\examples\rtos\CC2640R2_LAUNCHXL\blestack\simple_peripheral\src\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\icall\inc\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\inc\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\profiles\dev_info\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\profiles\roles\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\profiles\roles\cc26xx\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\profiles\simple_profile\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\profiles\simple_profile\cc26xx\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\target\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\hal\src\inc\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\hal\src\target\_common\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\hal\src\target\_common\cc26xx\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\devices\cc26x0r2\rom\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\heapmgr\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\icall\src\inc\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\osal\src\inc\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\services\src\saddr\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\services\src\sdata\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\devices\cc26x0r2\ \
-I BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ \
-Ohz" \
--linkOptions \
"--config_def CC2650=2 \
--config_def AUX_AS_RAM=1 \
--config BCDS_TEST_APP_SOURCE_PATH\TestApp\cc26xx_ble_app.icf \
--keep __vector_table -f BCDS_TEST_APP_SOURCE_PATH\config\configPkg\linker.cmd \
-f BCDS_TEST_APP_SOURCE_PATH\TestApp\IAR-Boundary.xcl \
BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\blestack\rom\ble_rom_releases\cc26xx_r2\Final_Release\common_r2.symbols \
BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\devices\cc26x0r2\driverlib\bin\iar\driverlib.lib \
BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\kernel\tirtos\packages\ti\dpl\lib\dpl_cc26x0r2.arm3 \
BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\drivers\lib\drivers_cc26x0r2.arm3 \
BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\drivers\rf\lib\rf_singleMode_cc26x0r2.arm3 \
BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\display\lib\display.arm3 \
BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\grlib\lib\grlib.arm3 \
BCDS_DEVELOPMENT_TOOLS_WIN\TI_BLE_Stack\v3.1.1-0.1.0\simplelink_cc2640r2_sdk_1_50_00_58\source\ti\mw\lcd\lib\lcd.arm3
--entry __iar_program_start --vfe --text_out locale"  \
--profile release \
--projFile "BCDS_TEST_APP_SOURCE_PATH\config" \
--cfgArgs={NO_ROM:1}  \
-o "BCDS_TEST_APP_SOURCE_PATH\config\configPkg"  \
"BCDS_TEST_APP_SOURCE_PATH\config\appBLE.cfg"