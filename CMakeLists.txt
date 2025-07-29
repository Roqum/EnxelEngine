cmake_minimum_required(VERSION 3.16)
project(EnxelEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(ENXEL_OUTPUT_DIR "${OUTPUT_DIR}/Enxel")
set(SANDBOX_OUTPUT_DIR "${OUTPUT_DIR}/Sandbox")

add_subdirectory(Enxel)
add_subdirectory(Sandbox)