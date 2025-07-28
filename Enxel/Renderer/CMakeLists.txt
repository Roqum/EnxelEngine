add_library(RendererAPI STATIC
    IRenderer.h
)

target_include_directories(IRenderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)