#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")

add_library(Renderer INTERFACE)

target_include_directories(Renderer INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Vulkan)



