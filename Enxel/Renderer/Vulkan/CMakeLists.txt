
file(GLOB VULKAN_SRC CONFIGURE_DEPENDS *.cpp *.h)

add_library(VulkanRenderer STATIC ${VULKAN_SRC})

find_package(Vulkan REQUIRED)

target_include_directories(VulkanRenderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/stb
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(VulkanRenderer PRIVATE Vulkan::Vulkan Renderer)

