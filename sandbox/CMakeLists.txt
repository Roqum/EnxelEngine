file(GLOB_RECURSE SANDBOX_SRC CONFIGURE_DEPENDS
    src/*.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>")

add_executable(Sandbox ${SANDBOX_SRC})

target_include_directories(Sandbox PUBLIC include)
target_link_libraries(Sandbox PRIVATE Enxel)


# Copy Enxel DLL after build
#add_custom_command(TARGET Sandbox POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIG>/$<TARGET_FILE_NAME:Enxel>"
#        "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>/"
#    COMMENT "Copying Enxel.dll to Sandbox output..."
#)

# Copy Vulkan DLL after build
#add_custom_command(TARGET Sandbox POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIG>/$<TARGET_FILE_NAME:VulkanRenderer>"
#        "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>/"
#    COMMENT "Copying VulkanRenderer.dll to Sandbox output..."
#)

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIG>/
        ${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>/
    COMMENT "Copying Enxel bin to Sandbox bin..."
)