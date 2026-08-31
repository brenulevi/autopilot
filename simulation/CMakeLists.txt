add_executable(helius_sim
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(helius_sim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(helius_sim PRIVATE 
    helius_autopilot
)

if(HELIUS_SIMULATION_TARGET STREQUAL "JSBSIM")
    add_subdirectory(backends/jsbsim)

    target_compile_definitions(helius_jsbsim_backend PRIVATE
        HELIUS_JSBSIM_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/vendor/jsbsim"
    )

    target_link_libraries(helius_sim PRIVATE
        helius_jsbsim_backend
    )

    target_compile_definitions(helius_sim
        PRIVATE
            HELIUS_SIM_BACKEND_JSBSIM
    )

else()
    message(FATAL_ERROR "Unsupported simulation target: ${HELIUS_SIMULATION_TARGET}.")
endif()

target_link_libraries(helius_sim PRIVATE
    helius_autopilot
)