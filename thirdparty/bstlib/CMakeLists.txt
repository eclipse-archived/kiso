cmake_minimum_required(VERSION 3.6)

project (bstlib C)

set(BST_PATH 3rd-party/bstlib)

# Only compile with proper toolchain
if(${CMAKE_CROSSCOMPILING})
   add_library(bstlib STATIC
      ${BST_PATH}/BMA2x2_driver/bma2x2.c
      ${BST_PATH}/BME280_driver/bme280.c
      ${BST_PATH}/BMG160_driver/bmg160.c
      ${BST_PATH}/BMI160_driver/bmi160.c
      ${BST_PATH}/BMM050_driver/bmm050.c
      ${BST_PATH}/BMP180_driver/bmp180.c
      ${BST_PATH}/BMP280_driver/bmp280.c
      ${BST_PATH}/BNO055_driver/bno055.c
   )
   target_include_directories(bstlib
   PUBLIC
      ${BST_PATH}/BMA2x2_driver
      ${BST_PATH}/BME280_driver
      ${BST_PATH}/BMG160_driver
      ${BST_PATH}/BMI160_driver
      ${BST_PATH}/BMM050_driver
      ${BST_PATH}/BMP180_driver
      ${BST_PATH}/BMP280_driver
      ${BST_PATH}/BNO055_driver
   )
   target_compile_options(bstlib PRIVATE
       -Wno-error
       -Wno-pointer-compare
       -Wno-misleading-indentation
       -Wno-extra
       -Wno-unused-const-variable)
endif()

if(${CMAKE_TESTING_ENABLED})
   add_library(bstlib_test_int INTERFACE)
   target_include_directories(bstlib_test_int
   INTERFACE
      test/unit/include
      ${BST_PATH}/BMA2x2_driver
      ${BST_PATH}/BME280_driver
      ${BST_PATH}/BMG160_driver
      ${BST_PATH}/BMI160_driver
      ${BST_PATH}/BMM050_driver
      ${BST_PATH}/BMP180_driver
      ${BST_PATH}/BMP280_driver
      ${BST_PATH}/BNO055_driver
   )

endif()
