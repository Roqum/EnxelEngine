#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")

file(GLOB_RECURSE Renderer_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(Renderer STATIC ${Renderer_SRC})

target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Vulkan REQUIRED)
target_link_libraries(Renderer PUBLIC SDL3::SDL3 Vulkan::Vulkan )

#target_precompile_headers(Renderer REUSE_FROM pchCommon)



#add_subdirectory(Vulkan)



