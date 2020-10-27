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

set(Xenomai_additionnal_INCLUDE_DIR "")
if(${CURRENT_OS} MATCHES "xenomai")
  set(Xenomai_additionnal_INCLUDE_DIR /usr/local/rtnet/include
                                      /usr/src/rtnet/stack/include)
endif()

set(all_targets)
set(all_target_exports)

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
# Export the target.
ament_export_interfaces(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
# For the installation
list(APPEND all_targets ${PROJECT_NAME})
list(APPEND all_target_exports export_${PROJECT_NAME})

#
# Building some utilities
#
if(Xenomai_FOUND)
  add_executable(test_sensor_stream tests/test_sensor_stream.cpp)
  target_link_libraries(test_sensor_stream ${PROJECT_NAME})
  list(APPEND all_targets test_sensor_stream)
endif()

add_executable(test_sensor tests/test_sensor.cpp)
target_link_libraries(test_sensor ${PROJECT_NAME})
list(APPEND all_targets test_sensor)

#
# building documentation
#
add_documentation()

#
# Install the package
#
install(DIRECTORY include/ DESTINATION include)

install(
  TARGETS ${all_targets}
  EXPORT ${all_target_exports}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
  INCLUDES
  DESTINATION include)

#
# Export the package as ament
#
ament_package()
