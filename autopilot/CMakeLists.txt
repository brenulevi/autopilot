add_library(helius_autopilot STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/autopilot.c
)

add_library(helius_autopilot::helius_autopilot ALIAS helius_autopilot)

target_include_directories(helius_autopilot PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(helius_autopilot PUBLIC
    $<$<CONFIG:Debug>:HELIUS_DEBUG>
    HELIUS_VERSION="${PROJECT_VERSION}"
    HELIUS_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    HELIUS_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    HELIUS_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)