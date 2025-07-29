file(GLOB_RECURSE ENXEL_SRC CONFIGURE_DEPENDS
    src/*.cpp
    include/*.h
)

add_library(Enxel SHARED ${ENXEL_SRC})

#target_link_libraries(Enxel PRIVATE Renderer)

target_include_directories(Enxel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(Enxel PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Enxel"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Enxel"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Enxel"
)

add_subdirectory(Renderer/Vulkan)