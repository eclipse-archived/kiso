cmake_minimum_required(VERSION 3.6)

project("Kiso Integration Test Executor" C)

if(${CMAKE_CROSSCOMPILING})
   set(APP_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/testapp/config PARENT_SCOPE)

   add_executable(TestExecutor
      testapp/source/main.c
   )

   target_include_directories(TestExecutor PRIVATE source)
   target_link_libraries(TestExecutor bsp essentials testing utils testentry ${KISO_OS_LIB} ${KISO_BOARD_LIBS})
   target_compile_definitions(TestExecutor PRIVATE ${KISO_BOARD_NAME})

   add_custom_target(TestExecutor.bin ALL
      COMMAND ${CMAKE_OBJCOPY} -O binary -R .usrpg $<TARGET_FILE:TestExecutor> ${CMAKE_CURRENT_BINARY_DIR}/TestExecutor.bin
      COMMENT "Creating flashable binary ${CMAKE_CURRENT_BINARY_DIR}/TestExecutor.bin"
   )
   add_dependencies(TestExecutor.bin TestExecutor)

   include(FlashTarget)
   CREATE_FLASH_TARGET_JLINK(TestExecutor)
endif()
