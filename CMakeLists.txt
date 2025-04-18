project(multimidia)
cmake_minimum_required(VERSION 3.10)

add_library(encoder STATIC encoder.cpp)
add_library(decoder STATIC decoder.cpp)
add_library(compressor STATIC compressor.cpp)

add_executable(arithmetic_coding main.cpp)

target_link_libraries(arithmetic_coding PRIVATE encoder decoder compressor)

target_compile_definitions(arithmetic_coding PRIVATE EXPECTS_CLI_ARGS)

add_custom_target(output ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:arithmetic_coding> ${CMAKE_BINARY_DIR}/output
    DEPENDS arithmetic_coding
)
