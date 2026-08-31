cmake_minimum_required(VERSION 3.20)

set(HELIUS_TARGET "SIMULATION" CACHE STRING "Target platform for Helius Autopilot")
set_property(CACHE HELIUS_TARGET PROPERTY STRINGS "STM32" "SIMULATION")

set(HELIUS_SIMULATION_TARGET "JSBSIM" CACHE STRING "Simulation target for Helius Autopilot")
set_property(CACHE HELIUS_SIMULATION_TARGET PROPERTY STRINGS "JSBSIM")

if(HELIUS_TARGET STREQUAL "STM32")
    if(NOT CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/firmware/cmake/gcc-arm-none-eabi.cmake" CACHE FILEPATH "Toolchain file for STM32" FORCE)
    endif()
elseif(HELIUS_TARGET STREQUAL "SIMULATION")
    # Simulation target specific configurations can be added here
else()
    message(FATAL_ERROR "Unsupported target platform: ${HELIUS_TARGET}.")
endif()

project(HeliusAutopilot VERSION 0.1.0 LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

message(STATUS "Helius Autopilot Target: ${HELIUS_TARGET}")
message(STATUS "Helius Autopilot Simulation Target: ${HELIUS_SIMULATION_TARGET}")
message(STATUS "CMake Toolchain File: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# Autopilot source directory
add_subdirectory(autopilot)

if(HELIUS_TARGET STREQUAL "STM32")

    add_subdirectory(firmware)

    target_link_libraries(firmware helius_autopilot)

    find_program(STM32_PROGRAMMER_CLI
        NAMES STM32_Programmer_CLI
        PATHS
            "$ENV{HOME}/.local/share/stm32cube/bundles/programmer"
            "/opt/st"
        PATH_SUFFIXES
            bin
            "2.23.0/bin"
            "tools/bin"
        DOC "STM32CubeProgrammer CLI for ST-Link flashing"
    )

     if(STM32_PROGRAMMER_CLI)
        add_custom_target(flash
            COMMAND ${STM32_PROGRAMMER_CLI}
                -c port=SWD
                -w $<TARGET_FILE:firmware>
                -v
                -rst
            DEPENDS firmware
            COMMENT "Flashing firmware.elf via ST-Link (SWD)"
            USES_TERMINAL
            VERBATIM
        )
        message(STATUS "Flash tool: ${STM32_PROGRAMMER_CLI}")
    else()
        message(WARNING "STM32_Programmer_CLI not found; 'flash' target disabled")
    endif()

elseif(HELIUS_TARGET STREQUAL "SIMULATION")
    add_subdirectory(simulation)
endif()
