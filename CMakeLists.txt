cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(log_test C CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/common.cmake)

# Include directories
include_directories(${CMAKE_PREFIX_PATH}/include)

# Find grpc_proto_lib
find_library(GRPC_PROTO_LIB grpc_proto_lib PATHS ${CMAKE_PREFIX_PATH}/lib)
if (NOT GRPC_PROTO_LIB)
    message(FATAL_ERROR "Not found GRPC_PROTO Library.")
endif()

# Find logger library
find_library(LOGGER_LIB logger_lib PATHS ${CMAKE_PREFIX_PATH}/lib)
if (NOT LOGGER_LIB)
    message(FATAL_ERROR "Not found Logger Library.")
endif()

# Create executable
add_executable(log_subscriber log_subscriber.cpp)

# Link libraries
target_link_libraries(log_subscriber 
    ${GRPC_PROTO_LIB}
    ${LOGGER_LIB}
    ${_CORE_LIBRARIES}
    pthread
)
