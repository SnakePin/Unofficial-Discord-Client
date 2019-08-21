# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.2)

# Set the project name and make rest of the file a project, this enables more CMake features
project (Unofficial-Discord-Client)

# If build type is not specified then fall back to Release config
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set required C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Manually add C++17 support to CMake if version < 3.8
if(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
    set(CMAKE_CXX17_STANDARD_COMPILE_OPTION "-std=c++17")
    set(CMAKE_CXX17_EXTENSION_COMPILE_OPTION "-std=gnu++17")
  elseif (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1)
    set(CMAKE_CXX17_STANDARD_COMPILE_OPTION "-std=c++1z")
    set(CMAKE_CXX17_EXTENSION_COMPILE_OPTION "-std=gnu++1z")
  endif()
endif()


# This needs to be here, before any add_executable calls and after project
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/output/archive")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/output/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/output/bin")

# Add all files
file(GLOB_RECURSE SOURCES
  "include/*.h"
  "include/*.hpp"
  "src/*.c"
  "src/*.cpp"
)

# Add an executable with the above sources
add_executable(Unofficial-Discord-Client ${SOURCES})

set(PROJ_OPTIONS "-Wall -Wextra")
set(PROJ_DEBUG_OPTIONS "")
set(PROJ_RELEASE_OPTIONS "-O3")
set(PROJ_LINKER_OPTIONS "")

# Only add debug linker option to this project because we only need debug information for this project
if (WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(PROJ_OPTIONS "/Wall")
  if(CMAKE_BUILD_TYPE EQUAL Debug OR CMAKE_BUILD_TYPE EQUAL RelWithDebInfo)
    set(PROJ_LINKER_OPTIONS "/DEBUG:FULL")
  endif()
elseif(WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  if(CMAKE_BUILD_TYPE EQUAL Debug OR CMAKE_BUILD_TYPE EQUAL RelWithDebInfo)
	set(PROJ_LINKER_OPTIONS "/debug:full")
  endif()
else()
  set(PROJ_DEBUG_OPTIONS "-g")
endif()

target_compile_options(Unofficial-Discord-Client PUBLIC "${PROJ_OPTIONS}")
target_compile_options(Unofficial-Discord-Client PUBLIC "$<$<CONFIG:DEBUG>:${PROJ_DEBUG_OPTIONS}>")
target_compile_options(Unofficial-Discord-Client PUBLIC "$<$<CONFIG:RELEASE>:${PROJ_RELEASE_OPTIONS}>")
set_target_properties(Unofficial-Discord-Client PROPERTIES LINK_FLAGS "${PROJ_LINKER_OPTIONS}")


target_include_directories(Unofficial-Discord-Client PUBLIC "${PROJECT_SOURCE_DIR}/include")

# rapidjson include path
set(RAPIDJSON_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/rapidjson/include")
target_include_directories(Unofficial-Discord-Client PUBLIC "${RAPIDJSON_INCLUDE_PATH}")

# Add CPR
# Go away tests, go away!
set(BUILD_CPR_TESTS OFF)
add_subdirectory(external/cpr)
target_include_directories(Unofficial-Discord-Client PUBLIC "${CPR_INCLUDE_DIRS}")
target_link_libraries(Unofficial-Discord-Client "${CPR_LIBRARIES}")

# Add OpenSSL
find_package(OpenSSL REQUIRED)
target_include_directories(Unofficial-Discord-Client PUBLIC "${OPENSSL_INCLUDE_DIR}")
target_link_libraries(Unofficial-Discord-Client "${OPENSSL_LIBRARIES}")

# Use standalone asio
target_compile_definitions(Unofficial-Discord-Client PUBLIC USE_STANDALONE_ASIO ASIO_STANDALONE)
