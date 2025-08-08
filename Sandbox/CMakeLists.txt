file(GLOB_RECURSE SANDBOX_SRC CONFIGURE_DEPENDS
    src/*.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>")

add_executable(Sandbox ${SANDBOX_SRC})

# Configure IDE to run with -C flag pointing to assets
if(WIN32)
    # Visual Studio
    set_target_properties(Sandbox PROPERTIES
        VS_DEBUGGER_COMMAND_ARGUMENTS "-C \"${CMAKE_SOURCE_DIR}/assets\""
    )
endif()

if(APPLE)
    # Xcode
    set_target_properties(Sandbox PROPERTIES
        XCODE_SCHEME_ARGUMENTS "-C \"${CMAKE_SOURCE_DIR}/Assets\""
    )
endif()

# For CLion on Linux (and other IDEs that support it)
set_target_properties(Sandbox PROPERTIES
    VS_DEBUGGER_COMMAND_ARGUMENTS "-C \"${CMAKE_SOURCE_DIR}/Assets\""
)

target_include_directories(Sandbox PUBLIC include)
target_link_libraries(Sandbox PRIVATE Enxel)


# Copy Enxel DLL after build
#add_custom_command(TARGET Sandbox POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIG>/$<TARGET_FILE_NAME:SDL3>"
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

# Run shader compile batch file after build
#add_custom_command(
#    TARGET Sandbox
#    POST_BUILD
#    COMMAND "${CMAKE_SOURCE_DIR}/Sandbox/shaders/compile.bat"
#    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Sandbox/shaders"
#    COMMENT "Compiling shaders..."
#)

# Post-build step to copy the assets folder
#add_custom_command(
#    TARGET Sandbox
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#            "${CMAKE_SOURCE_DIR}/Sandbox/assets"
#            "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>/assets/"
#        COMMENT "Copying Enxel bin to Sandbox bin..."
#)

# Copy shaders folder after build
#add_custom_command(
#    TARGET Sandbox
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#            "${CMAKE_SOURCE_DIR}/Sandbox/shaders"
#            "${CMAKE_BINARY_DIR}/bin/Sandbox/$<CONFIG>/shaders"
#    COMMENT "Copying shaders folder..."
#)

