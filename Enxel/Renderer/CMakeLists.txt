add_library(Renderer STATIC
    IRenderer.h
)

target_include_directories(Renderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#target_link_libraries(Renderer PUBLIC
#    VulkanRenderer
#)

