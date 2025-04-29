

file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS src/*.cpp include/*.h)

add_library(engine STATIC ${ENGINE_SRC})

target_include_directories(engine PUBLIC include)

target_link_libraries(engine PUBLIC glm)  
#target_link_libraries(engine PUBLIC SDL)  


# Find Vulkan (uses VULKAN_SDK env variable)
find_package(Vulkan REQUIRED)

# Link Vulkan to your engine
target_link_libraries(engine PUBLIC Vulkan::Vulkan)