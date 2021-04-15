#
# Copyright (c) 2019, New York University and Max Planck Gesellschaft.
#
# License BSD-3 clause
#

#
# set up the project
#
cmake_minimum_required(VERSION 3.10.2)

project(ati_ft_sensor)

# Using C++17
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

#
# Dependencies
#

# depend on ament macros
find_package(ament_cmake REQUIRED)
find_package(mpi_cmake_modules REQUIRED)
find_package(real_time_tools REQUIRED)
find_package(Boost REQUIRED)

set(Xenomai_additionnal_INCLUDE_DIR)
if(${CURRENT_OS} MATCHES "xenomai")
  set(Xenomai_additionnal_INCLUDE_DIR /usr/local/rtnet/include
                                      /usr/src/rtnet/stack/include)
endif()

# export de dependencies
ament_export_dependencies(Boost real_time_tools)
# Export the target.
ament_export_interfaces(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

set(all_lib_targets)
set(all_exe_targets)

#
# Add the main library
#
# Create the CMake target
add_library(${PROJECT_NAME} SHARED src/AtiFTSensor.cpp)
# Add the include dependencies
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include> ${Xenomai_INCLUDE_DIR}
         ${Xenomai_additionnal_INCLUDE_DIR})
# Link the dependencies
ament_target_dependencies(${PROJECT_NAME} Boost)
ament_target_dependencies(${PROJECT_NAME} Threads)
target_link_libraries(${PROJECT_NAME} real_time_tools::real_time_tools)
target_link_libraries(${PROJECT_NAME} ${Xenomai_LIBS})
# For the installation
list(APPEND all_lib_targets ${PROJECT_NAME})

#
# Building some utilities
#
if(Xenomai_FOUND)
  add_executable(ati_ft_sensor_test_sensor_stream tests/test_sensor_stream.cpp)
  target_link_libraries(ati_ft_sensor_test_sensor_stream ${PROJECT_NAME})
  list(APPEND all_exe_targets ati_ft_sensor_test_sensor_stream)
endif()

add_executable(ati_ft_sensor_test_sensor tests/test_sensor.cpp)
target_link_libraries(ati_ft_sensor_test_sensor ${PROJECT_NAME})
list(APPEND all_exe_targets ati_ft_sensor_test_sensor)

#
# building documentation
#
add_documentation()

#
# Install the package
#
install(DIRECTORY include/ DESTINATION include)

install(
  TARGETS ${all_lib_targets} ${all_exe_targets}
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
  INCLUDES
  DESTINATION include)

foreach(exe_target ${all_exe_targets})
  # Install a symlink of the executable in lib/ati_ft_sensor
  string(
    CONCAT symlink_command
          "execute_process("
          "  COMMAND ${CMAKE_COMMAND} -E make_directory "
          "  ${CMAKE_INSTALL_PREFIX}/bin/)\n"
          "execute_process("
          "  COMMAND ${CMAKE_COMMAND} -E create_symlink "
          "  ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME}/${exe_target}"
          "  ${CMAKE_INSTALL_PREFIX}/bin/${exe_target})"
  )
  install(CODE ${symlink_command})
endforeach()
  
#
# Export the package as ament
#
ament_package()
