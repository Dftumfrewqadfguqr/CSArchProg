cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME HTTPServer)
project(${PROJECT_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

if (WIN32)
    set(CMAKE_CXX_FLAGS "-lgdi32 -luser32 -lkernel32 -static")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wempty-body -Wredundant-move -O2")

add_executable(${PROJECT_NAME}
        main.cpp
        GlobalErrors.cpp
        Api/HTTPServer.cpp
        Api/Routers.cpp
        Users/UsersStorage.cpp
        Users/User.cpp
        JsonCast/JsonCast.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ROOT}/src/Server/)

# outputs
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/build/)
