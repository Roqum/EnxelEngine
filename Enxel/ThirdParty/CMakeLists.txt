
#set_target_properties(SDL3 PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${ENXEL_BIN_DEBUG}"
#    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ENXEL_BIN_RELEASE}"
#    LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${ENXEL_BIN_DEBUG}"
#    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${ENXEL_BIN_RELEASE}"
#    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${ENXEL_BIN_DEBUG}"
#   ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${ENXEL_BIN_RELEASE}"
#)
include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Enxel/$<CONFIGURATION>")

# === GLM ===
#FetchContent_Declare(
#    glm
#   GIT_REPOSITORY https://github.com/g-truc/glm.git
#   GIT_TAG latest  # or latest stable tag
#)
#FetchContent_Populate(glm)

# === SDL3 ===
FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.18  # adjust to the correct tag
)
FetchContent_MakeAvailable(SDL)