file(GLOB_RECURSE SANDBOX_SRC CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_executable(sandbox ${SANDBOX_SRC})

target_include_directories(sandbox PUBLIC include)
target_link_libraries(sandbox PRIVATE engine)