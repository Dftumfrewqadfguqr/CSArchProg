cmake_minimum_required(VERSION 3.0)
project(CSArchProg)
cmake_policy(SET CMP0079 NEW)

set(LIBHV_INCLUDE ${PROJECT_BINARY_DIR}/external/libhv/libhv/include/hv)
#set(GT_INCLUDE ${PROJECT_BINARY_DIR}/external/googletest/googletest/include)
find_package(OpenSSL REQUIRED)

#libhv
include_directories(${LIBHV_INCLUDE})
add_subdirectory(${PROJECT_SOURCE_DIR}/external/libhv)

#google test
#include_directories(${GT_INCLUDE})
#add_subdirectory(${PROJECT_SOURCE_DIR}/external/googletest)

#Utility
include_directories(${PROJECT_SOURCE_DIR}/src/Utility)
add_library(Utility STATIC src/Utility/Utility.cpp)

#HTTPClient
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Client)
target_link_libraries(HTTPClient ${OPENSSL_LIBRARIES})
target_link_libraries(HTTPClient hv_static Utility)

#HTTPServer
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Server)
target_link_libraries(HTTPServer ${OPENSSL_LIBRARIES})
target_link_libraries(HTTPServer hv_static Utility)
