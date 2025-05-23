project(multimidia)
cmake_minimum_required(VERSION 3.10)

add_library(encoder STATIC encoder.cpp)
add_library(decoder STATIC decoder.cpp)
add_library(compressor STATIC compressor.cpp)

add_executable(arith_codec main.cpp)

target_link_libraries(arith_codec PRIVATE encoder decoder compressor)

target_compile_definitions(arith_codec PRIVATE EXPECTS_CLI_ARGS)

add_custom_target(output ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:arith_codec> ${CMAKE_BINARY_DIR}/output
    DEPENDS arith_codec
)
