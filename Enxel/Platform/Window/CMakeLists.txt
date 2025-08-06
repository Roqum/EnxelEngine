file(GLOB PLATFORM_WINDOW_SRC
    Window.cpp
    Window.h
)

if(WIN32)
    file(GLOB PLATFORM_SRC
        Windows/*.cpp
        Windows/*.h
    )
elseif(UNIX)
    #TODO 
    #file(GLOB PLATFORM_SRC
    #    Linux/*.cpp
    #    Linux/*.h
    #)
endif()

list(APPEND PLATFORM_WINDOW_SRC ${PLATFORM_SRC})

add_library(PlatformWindow STATIC ${PLATFORM_WINDOW_SRC})

target_link_libraries(PlatformWindow PUBLIC SDL3::SDL3)

target_include_directories(PlatformWindow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
