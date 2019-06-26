# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Change this to appropriate configuration (Release or Debug)
set(CMAKE_BUILD_TYPE Debug)

# If build type is not specified then fall back to Release config
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Set the project name
project (Unofficial-Discord-Client)

# This needs to be here, before any add_executable calls and after project
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/bin)

# Add all files
file(GLOB_RECURSE SOURCES
  "include/*.h"
  "include/*.hpp"
  "src/*.c"
  "src/*.cpp"
)

# Add an executable with the above sources
add_executable(Unofficial-Discord-Client ${SOURCES})

target_include_directories(Unofficial-Discord-Client PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Add CPR
# Go away tests, go away!
set(BUILD_CPR_TESTS OFF)
add_subdirectory(cpr)
target_include_directories(Unofficial-Discord-Client PUBLIC ${CPR_INCLUDE_DIRS})
target_link_libraries(Unofficial-Discord-Client ${CPR_LIBRARIES})

# Add OpenSSL
find_package(OpenSSL REQUIRED)
target_include_directories(Unofficial-Discord-Client PUBLIC ${OPENSSL_INCLUDE_DIR})
target_link_libraries(Unofficial-Discord-Client ${OPENSSL_LIBRARIES})

## Add asio
#find_package(libasio REQUIRED)
#include_directories(${LIBASIO_INCLUDE_DIR})
#target_link_libraries(Unofficial-Discord-Client ${LIBASIO_LIBRARIES})

# Use standalone asio (most useful comment btw)
target_compile_definitions(Unofficial-Discord-Client PUBLIC USE_STANDALONE_ASIO)

## Add libcrypto
#find_package(libcrypto REQUIRED)
#include_directories(${LIBCRYPTO_INCLUDE_DIR})
#target_link_libraries(Unofficial-Discord-Client ${LIBCRYPTO_LIBRARIES})

