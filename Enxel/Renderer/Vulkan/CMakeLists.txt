
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


find_package(Vulkan REQUIRED)
target_link_libraries(VulkanRenderer PRIVATE Vulkan::Vulkan)