file(GLOB_RECURSE SANDBOX_SRC CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(Sandbox ${SANDBOX_SRC})

target_include_directories(Sandbox PUBLIC include)
target_link_libraries(Sandbox PRIVATE Enxel)

set_target_properties(Sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Sandbox"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Sandbox"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Sandbox"
)

# Copy Enxel DLL after build
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Enxel/$<TARGET_FILE_NAME:Enxel>"
        "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Sandbox"
    COMMENT "Copying Enxel.dll to Sandbox output..."
)

# Copy Vulkan DLL after build
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Enxel/$<TARGET_FILE_NAME:VulkanRenderer>"
        "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Sandbox"
    COMMENT "Copying VulkanRenderer.dll to Sandbox output..."
)