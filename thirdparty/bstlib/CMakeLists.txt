cmake_minimum_required(VERSION 3.6)

project (bstlib C)

set(BST_DIR 3rd-party/bstlib)

# Only compile with proper toolchain
if(${CMAKE_CROSSCOMPILING})
   add_library(bstlib STATIC
      ${BST_DIR}/BMA2x2_driver/bma2x2.c
      ${BST_DIR}/BME280_driver/bme280.c
      ${BST_DIR}/BMG160_driver/bmg160.c
      ${BST_DIR}/BMI160_driver/bmi160.c
      ${BST_DIR}/BMM050_driver/bmm050.c
      ${BST_DIR}/BMP180_driver/bmp180.c
      ${BST_DIR}/BMP280_driver/bmp280.c
      ${BST_DIR}/BNO055_driver/bno055.c
   )
   target_include_directories(bstlib
   PUBLIC
      ${BST_DIR}/BMA2x2_driver
      ${BST_DIR}/BME280_driver
      ${BST_DIR}/BMG160_driver
      ${BST_DIR}/BMI160_driver
      ${BST_DIR}/BMM050_driver
      ${BST_DIR}/BMP180_driver
      ${BST_DIR}/BMP280_driver
      ${BST_DIR}/BNO055_driver
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
      ${BST_DIR}/BMA2x2_driver
      ${BST_DIR}/BME280_driver
      ${BST_DIR}/BMG160_driver
      ${BST_DIR}/BMI160_driver
      ${BST_DIR}/BMM050_driver
      ${BST_DIR}/BMP180_driver
      ${BST_DIR}/BMP280_driver
      ${BST_DIR}/BNO055_driver
   )

endif()
