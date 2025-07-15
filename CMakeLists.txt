cmake_minimum_required(VERSION 3.10)
project(lora)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)

# Ruta correcta a la carpeta del submódulo con los .proto
set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/EasyLoRa_Proto)

set(PROTO_GEN_DIR ${CMAKE_BINARY_DIR}/proto)

file(MAKE_DIRECTORY ${PROTO_GEN_DIR})

file(GLOB PROTO_FILES "${PROTO_SRC_DIR}/*.proto")
message(STATUS "Proto files found: ${PROTO_FILES}")

if(PROTO_FILES)
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
else()
    message(WARNING "No .proto files found in ${PROTO_SRC_DIR}")
endif()

include_directories(${PROTO_GEN_DIR})
include_directories(external/serial/include)

add_executable(lora
    main.cpp
    external/serial/src/serial.cc
    external/serial/src/impl/unix.cc
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

target_link_libraries(lora pthread protobuf::libprotobuf)
target_include_directories(lora PRIVATE ${PROTO_GEN_DIR})