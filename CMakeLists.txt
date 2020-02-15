# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.8)

# Set the project name
project(Unofficial-Discord-Client)

# If build type is not specified then fallback to Release config
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Prevent in-source builds
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) named \"build\" and run CMake from there. You may need to remove CMakeCache.txt.")
endif()

# Add our own CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

# This needs to be here, before any add_executable calls and after project
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/output/archive")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/output/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/output/bin")

# Add all files
file(GLOB_RECURSE SOURCES "src/*.c" "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Add an executable with the above sources
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set required C++ version, this needs to be here, after the add_executable call
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Enable warnings and set required compiler flags
if (WIN32)
  if(MSVC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    target_compile_options(${PROJECT_NAME} PRIVATE "/W4")
  endif()
  # Set Windows SDK version to Windows XP
  target_compile_definitions(${PROJECT_NAME} PRIVATE "_WIN32_WINNT=0x0501")
else()
  target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Add cpprestsdk
find_package(cpprestsdk CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE cpprestsdk::cpprest)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE cpprestsdk:cpprest)

# Add ASIO
# find_package(asio CONFIG REQUIRED) - This causes issues on linux
find_path(LIBASIO_INCLUDE_DIR "asio/basic_io_object.hpp")
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${LIBASIO_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio)
#target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE asio::asio)

# Add SDL2
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE SDL2::SDL2)

# Add OpenGL (for imgui OpenGL implementation)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE OpenGL::GL)

# Add RapidJSON (header only lib)
find_package(RapidJSON CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${RAPIDJSON_INCLUDE_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE "RAPIDJSON_HAS_STDSTRING")

# Add TCLAP (has no CMake integration) (header only lib)
find_path(TCLAP_INCLUDE_DIR "tclap/CmdLine.h")
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${TCLAP_INCLUDE_DIR})

# Add tinyformat (has no CMake integration) (header only lib)
set(TINYFORMAT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/tinyformat")
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${TINYFORMAT_INCLUDE_DIR})
