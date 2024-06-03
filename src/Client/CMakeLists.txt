cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME HTTPClient)
project(${PROJECT_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-lgdi32 -luser32 -lkernel32 -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wempty-body -Wredundant-move -O2")

add_executable(${PROJECT_NAME}
        main.cpp
        Client.cpp
)

# outputs
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/bin/)
