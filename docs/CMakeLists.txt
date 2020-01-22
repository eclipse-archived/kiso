
find_package(Doxygen)
if(${DOXYGEN_FOUND})
   SET(cmake_doxy_output_root ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
   SET(cmake_doxy_input_root ${CMAKE_CURRENT_SOURCE_DIR}/doxygen)
   configure_file(${cmake_doxy_input_root}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${cmake_doxy_input_root}/DoxygenLayout.xml ${CMAKE_CURRENT_BINARY_DIR}/DoxygenLayout.xml)
   add_custom_target(docs
      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
   )
else()
   message(WARNING "Doxygen is needed to build the documentation.")
endif()