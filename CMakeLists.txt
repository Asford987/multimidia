project(multimidia)
cmake_minimum_required(VERSION 3.10)

# Add executables for encode and decode
add_executable(encode encode.cpp)
add_executable(decode decode.cpp)

# Indicate that these executables expect CLI arguments
target_compile_definitions(encode PRIVATE EXPECTS_CLI_ARGS)
target_compile_definitions(decode PRIVATE EXPECTS_CLI_ARGS)

# Custom target to copy the executables to the output directory
add_custom_target(output ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:encode> ${CMAKE_BINARY_DIR}/output
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:decode> ${CMAKE_BINARY_DIR}/output
    DEPENDS encode decode
)