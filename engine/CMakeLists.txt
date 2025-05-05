file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS src/*.cpp include/*.h)

# Add shader directory path
set(SHADER_DIR src/shaders)
set(SHADER_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

# Copy shaders at build time
file(GLOB SHADER_FILES "${SHADER_DIR}/*.*")
file(COPY ${SHADER_FILES} DESTINATION ${SHADER_OUTPUT_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_library(engine STATIC ${ENGINE_SRC})

target_include_directories(engine PUBLIC include)

find_package(Vulkan REQUIRED)

# Link Vulkan to your engine
target_link_libraries(engine PUBLIC Vulkan::Vulkan SDL3::SDL3 glm::glm)

