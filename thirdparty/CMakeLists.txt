cmake_minimum_required(VERSION 3.6)

# Include just the OS we will use
add_subdirectory(${KISO_OS_LIB})

# Include the additional libraries for the board
foreach(lib ${KISO_BOARD_LIBS})
   add_subdirectory(${lib})
endforeach(lib ${KISO_BOARD_LIBS})

foreach(lib ${KISO_APP_LIBS})
   add_subdirectory(${lib})
endforeach(lib ${KISO_APP_LIBS})

# Add testing lib if needed
if(${CMAKE_TESTING_ENABLED})
   add_subdirectory(Gtest)
endif()
