# Add source to this project's executable.
add_executable (GameEngine 
  main.cpp
  vk_engine.cpp
  vk_engine.h
 )

set_property(TARGET GameEngine PROPERTY CXX_STANDARD 20)
set_property(TARGET GameEngine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:GameEngine>)

target_include_directories(GameEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(GameEngine vkguide_shared  vkbootstrap)

add_custom_command(TARGET GameEngine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:GameEngine> $<TARGET_FILE_DIR:GameEngine>
  COMMAND_EXPAND_LISTS
  )