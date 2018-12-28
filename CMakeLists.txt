cmake_minimum_required(VERSION 3.7)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# We are living in the 21st century...
add_compile_options(-Wall -Wextra)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

# render fonts
add_executable(render render.c)

# test program
add_executable(arduino arduino.cpp)

# copy the Python script to the build dir
add_custom_target(copy)
add_custom_command(TARGET copy PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E
                   copy ${CMAKE_SOURCE_DIR}/horizontal.py ${CMAKE_BINARY_DIR})
add_dependencies(arduino copy)
