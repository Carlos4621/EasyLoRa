cmake_minimum_required(VERSION 3.10)
project(lora)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
