cmake_minimum_required(VERSION 2.6)

project(ati_ft_sensor)
include(cmake/os_detection.cmake)

# This macro sets the C++ preprocessor flags "XENOMAI", "RT_PREEMPT", or
# "UBUNTU" according to the current operating system.
DEFINE_OS()

execute_process(
  COMMAND uname -a OUTPUT_VARIABLE UNAME_OUT)
string(TOLOWER "${UNAME_OUT}" OS_VERSION)

if(OS_VERSION MATCHES "xenomai")
  find_package(catkin REQUIRED COMPONENTS xenomai_ros mpi_cmake_modules real_time_tools)
else()
  find_package(catkin REQUIRED COMPONENTS mpi_cmake_modules real_time_tools)
endif()


#execute_process(COMMAND /usr/xenomai/bin/xeno-config --skin=native --cflags OUTPUT_VARIABLE XENO_NATIVE_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#execute_process(COMMAND /usr/xenomai/bin/xeno-config --skin=native --ldflags OUTPUT_VARIABLE XENO_NATIVE_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)


#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -Wall -O3 ${XENO_NATIVE_CFLAGS}")
#SET(CMAKE_VERBOSE_MAKEFILE on )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

find_package(Boost COMPONENTS system filesystem thread REQUIRED)

if(OS_VERSION MATCHES "xenomai")
  catkin_package(
    INCLUDE_DIRS include
    CATKIN_DEPENDS xenomai_ros
    LIBRARIES ${PROJECT_NAME}
  )

  INCLUDE_DIRECTORIES(include
    /usr/local/rtnet/include
    /usr/src/rtnet/stack/include
    ${catkin_INCLUDE_DIRS}
  )
else()
  catkin_package(
    INCLUDE_DIRS include
    LIBRARIES ${PROJECT_NAME}
  )

  INCLUDE_DIRECTORIES(include ${catkin_INCLUDE_DIRS})
endif()

if(Xenomai_FOUND)
  include_directories( ${Xenomai_INCLUDE_DIR} )
endif()

LINK_DIRECTORIES(#${PROJECT_NAME_BIN}/lib
${Boost_LIBRARY_DIR})

if(Xenomai_FOUND)
message(STATUS "Xenomai_INCLUDE_DIR: ${Xenomai_INCLUDE_DIR}")
  add_xenomai_flags()
endif()


#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_LIBRARY(${PROJECT_NAME} src/AtiFTSensor.cpp)
target_link_libraries(${PROJECT_NAME}
  real_time_tools)

if(Xenomai_FOUND)
  target_link_libraries(${PROJECT_NAME}
    ${Xenomai_LIBRARIES_NATIVE}
    ${Xenomai_LIBRARY_RTDM})

  ADD_EXECUTABLE(test_sensor
    tests/test_sensor.cpp)
  target_link_libraries(test_sensor
    ${PROJECT_NAME}
    ${Xenomai_LIBRARIES_NATIVE}
    ${Xenomai_LIBRARY_RTDM}
    ${Boost_LIBRARIES}
    )

  ADD_EXECUTABLE(test_sensor_stream
    tests/test_sensor.cpp)
  target_link_libraries(test_sensor_stream
    ${PROJECT_NAME}
    ${Xenomai_LIBRARIES_NATIVE}
    ${Xenomai_LIBRARY_RTDM}
    ${Boost_LIBRARIES}
    )
else()
  ADD_EXECUTABLE(test_sensor tests/test_sensor.cpp)
  target_link_libraries(test_sensor
    ${PROJECT_NAME}
    ${Boost_LIBRARIES}
    )
endif()

##########################
# building documentation #
##########################
build_doxygen_documentation()