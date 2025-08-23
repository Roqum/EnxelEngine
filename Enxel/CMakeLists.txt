file(GLOB_RECURSE ENXEL_SRC CONFIGURE_DEPENDS 
Enxel.cpp 
Enxel.h
Window/*.cpp
Window/*.h
World/*.cpp
World/*.h
Core/*.h
Core/*.cpp
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")

# Platform-specific Window implementation
if(WIN32)
    file(GLOB PLATFORM_WINDOW_SRC CONFIGURE_DEPENDS
        Window/Windows/*.cpp
        Window/Windows/*.h
    )
    list(APPEND ENXEL_SRC ${PLATFORM_WINDOW_SRC})
elseif(UNIX)
    # add Linux platform window files here
endif()

add_library(Enxel STATIC ${ENXEL_SRC})

target_include_directories(Enxel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_subdirectory(Platform)
add_subdirectory(ThirdParty)
add_subdirectory(Renderer)

#add_subdirectory(SDL EXCLUDE_FROM_ALL)  


target_link_libraries(Enxel PUBLIC SDL3::SDL3 glm::glm imgui)
target_link_libraries(Enxel PRIVATE Renderer PlatformWindow )


target_compile_definitions(Enxel PRIVATE
    $<$<CONFIG:Debug>:PROFILING=1>
    $<$<CONFIG:Release>:PROFILING=0>
)
target_compile_definitions(Renderer PRIVATE
    $<$<CONFIG:Debug>:PROFILING=1>
    $<$<CONFIG:Release>:PROFILING=0>
)
#target_precompile_headers(Enxel REUSE_FROM pchCommon)

#target_precompile_headers(Enxel INTERFACE ${ENXEL_PCH})