file(GLOB_RECURSE ENXEL_SRC CONFIGURE_DEPENDS
    src/*.cpp
    include/*.h
    Window/*.cpp
    Window/*.h
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")

add_library(Enxel SHARED ${ENXEL_SRC})


target_include_directories(Enxel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(ThirdParty)
add_subdirectory(Renderer/Vulkan)

#add_subdirectory(SDL EXCLUDE_FROM_ALL)  

target_link_libraries(Enxel PUBLIC SDL3::SDL3)


target_precompile_headers(Enxel PRIVATE "pchEnxel.h")