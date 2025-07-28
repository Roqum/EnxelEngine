#add_subdirectory(Core)
#add_subdirectory(Renderer)
#add_subdirectory(ThirdParty)
#add_subdirectory(World)

add_library(Enxel SHARED
    Enxel.cpp
 "Enxel.h")

target_link_libraries(Enxel PRIVATE IRenderer)
target_include_directories(Enxel PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(Enxel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

