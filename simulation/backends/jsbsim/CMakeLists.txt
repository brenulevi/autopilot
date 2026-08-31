add_subdirectory(../../vendor/jsbsim ${CMAKE_CURRENT_BINARY_DIR}/vendor/jsbsim)

add_library(helius_jsbsim_backend STATIC
    JSBSimAdapter.cpp
)

target_include_directories(helius_jsbsim_backend
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(helius_jsbsim_backend PRIVATE
    libJSBSim
)