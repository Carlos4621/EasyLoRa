cmake_minimum_required(VERSION 3.10)
project(lora)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -DDEBUG")
    message(STATUS "Debug mode enabled")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    message(STATUS "Release mode enabled")
endif()

find_package(Protobuf REQUIRED)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/EasyLoRa_Proto)

file(GLOB PROTO_FILES "${PROTO_SRC_DIR}/*.proto")
message(STATUS "Proto files found: ${PROTO_FILES}")

if(PROTO_FILES)
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
else()
    message(WARNING "No .proto files found in ${PROTO_SRC_DIR}")
endif()

include_directories(external/serial/include)
include_directories(include)

# Option to build as library or executable
option(BUILD_AS_LIBRARY "Build EasyLoRa as library instead of executable" OFF)

if(BUILD_AS_LIBRARY)
    # Create EasyLoRa library
    add_library(easylora STATIC
        external/serial/src/serial.cc
        external/serial/src/impl/unix.cc
        src/EasyLoRa.cpp
        src/ModuleConfig.cpp
        ${PROTO_SRCS}
        ${PROTO_HDRS}
    )

    target_include_directories(easylora PUBLIC 
        external/serial/include
        include
        ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_libraries(easylora PUBLIC pthread protobuf::libprotobuf)
    
    # Export variables for parent project
    set(EASYLORA_INCLUDE_DIRS 
        ${CMAKE_CURRENT_SOURCE_DIR}/external/serial/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
        PARENT_SCOPE)
    set(EASYLORA_LIBRARIES easylora pthread protobuf::libprotobuf PARENT_SCOPE)
else()
    # Original executable build
    add_executable(lora
        main.cpp
        external/serial/src/serial.cc
        external/serial/src/impl/unix.cc
        src/EasyLoRa.cpp
        src/ModuleConfig.cpp
        ${PROTO_SRCS}
        ${PROTO_HDRS}
    )

    target_include_directories(lora PRIVATE 
        external/serial/include
        include
        ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_libraries(lora pthread protobuf::libprotobuf)
endif()
