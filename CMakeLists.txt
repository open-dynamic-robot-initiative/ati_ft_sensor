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
find_package(mpi_cmake_modules REQUIRED)
find_package(real_time_tools REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(Eigen3 REQUIRED)
find_package(pybind11 REQUIRED)

set(Xenomai_additionnal_INCLUDE_DIR)
if(${CURRENT_OS} MATCHES "xenomai")
  set(Xenomai_additionnal_INCLUDE_DIR /usr/local/rtnet/include
                                      /usr/src/rtnet/stack/include)
endif()


set(all_targets)

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
target_link_libraries(${PROJECT_NAME} Boost::boost)
target_link_libraries(${PROJECT_NAME} Boost::thread)
target_link_libraries(${PROJECT_NAME} real_time_tools::real_time_tools)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} ${Xenomai_LIBS})
# For the installation
list(APPEND all_targets ${PROJECT_NAME})

#
# Building some utilities
#
if(Xenomai_FOUND)
  add_executable(ati_ft_sensor_test_sensor_stream tests/test_sensor_stream.cpp)
  target_link_libraries(ati_ft_sensor_test_sensor_stream ${PROJECT_NAME})
  list(APPEND all_targets ati_ft_sensor_test_sensor_stream)
endif()

add_executable(ati_ft_sensor_test_sensor tests/test_sensor.cpp)
target_link_libraries(ati_ft_sensor_test_sensor ${PROJECT_NAME})
list(APPEND all_targets ati_ft_sensor_test_sensor)

#
# building documentation
#
add_documentation()

# python bindings
set(py_ati_core_SRC_FILES
  srcpy/ati_ft_sensor_cpp.cpp
)

pybind11_add_module(ati_ft_sensor_cpp MODULE ${py_ati_core_SRC_FILES})
target_link_libraries(ati_ft_sensor_cpp PRIVATE pybind11::module)
target_link_libraries(ati_ft_sensor_cpp PRIVATE ati_ft_sensor)

# install the bindings
get_python_install_dir(python_install_dir)
install(TARGETS ati_ft_sensor_cpp DESTINATION ${python_install_dir})

# install python package
install(
  DIRECTORY python
  DESTINATION "${python_install_dir}"
  PATTERN "*.pyc" EXCLUDE 
  PATTERN "__pycache__" EXCLUDE
)

#
# Install the package
#

# install the include directory
install(DIRECTORY include/ DESTINATION include)

# command to install the library and binaries
install(
  TARGETS ${all_targets}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include)

# Export this package as a cmake package.
generate_cmake_package()