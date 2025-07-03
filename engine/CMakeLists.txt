file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS src/*.cpp include/*.h)

foreach(source_file ${ENGINE_SRC})
    # Get the directory of the file
    get_filename_component(source_path "${source_file}" PATH)

    # Make it relative to the source root so folders aren't absolute paths
    file(RELATIVE_PATH source_path_rel "${CMAKE_SOURCE_DIR}" "${source_path}")

    # Replace forward slashes with backslashes for Visual Studio folders
    string(REPLACE "/" "\\" group_name "${source_path_rel}")

    # Assign file to the group (folder) inside Visual Studio
    source_group("${group_name}" FILES "${source_file}")
endforeach()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_library(engine STATIC ${ENGINE_SRC} "include/World/Chunk.h")

target_include_directories(engine PUBLIC include)

find_package(Vulkan REQUIRED)

# Link Vulkan to your engine
target_link_libraries(engine PUBLIC Vulkan::Vulkan SDL3::SDL3 glm::glm)

#Set resource directories
set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(TEXTURES_DIR ${CMAKE_SOURCE_DIR}/textures)

set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)
set(TEXTURES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/assets)


# Copy shaders and textures
add_custom_target(copyResources
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copyResources.cmake
)
ADD_DEPENDENCIES ( engine copyResources)





