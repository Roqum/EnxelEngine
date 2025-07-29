
add_library(VulkanRenderer SHARED
    src/VulkanRenderer.cpp
    include/Vulkan/VulkanRenderer.h
)

target_include_directories(VulkanRenderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/..

)


set_target_properties(VulkanRenderer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug/Enxel"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release/Enxel"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug/Enxel"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release/Enxel"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug/Enxel"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release/Enxel"
)


find_package(Vulkan REQUIRED)
target_link_libraries(VulkanRenderer PRIVATE Vulkan::Vulkan)