#cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME ClientTest)
#project(${PROJECT_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

if (WIN32)
    set(CMAKE_CXX_FLAGS "-lgdi32 -luser32 -lkernel32 -static")
endif()

add_executable(${PROJECT_NAME}
        main.cpp
        )

# outputs
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/build/)
