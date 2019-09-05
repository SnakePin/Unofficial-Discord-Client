# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.8)

# Set the project name and make rest of the file a project, this enables more CMake features
project (Unofficial-Discord-Client)

# If build type is not specified then fallback to Release config
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
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
add_executable(Unofficial-Discord-Client "${SOURCES}")

# Set required C++ version, this needs to be here, after the add_executable call
target_compile_features(Unofficial-Discord-Client PUBLIC cxx_std_17)

target_compile_options(Unofficial-Discord-Client PRIVATE "$<$<CONFIG:RELEASE>:-O3>")
# Only add debug linker option to this project because we only need debug information for this project
if (WIN32 AND MSVC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  target_compile_options(Unofficial-Discord-Client PRIVATE "/W4")
  set_target_properties(Unofficial-Discord-Client PROPERTIES LINK_FLAGS "$<$<CONFIG:DEBUG>:/DEBUG:FULL>")
else()
  target_compile_options(Unofficial-Discord-Client PRIVATE "-Wall" "-Wextra")
  target_compile_options(Unofficial-Discord-Client PRIVATE "$<$<CONFIG:DEBUG>:-g>")
endif()


target_include_directories(Unofficial-Discord-Client PUBLIC "${PROJECT_SOURCE_DIR}/include")

# rapidjson include path
set(RAPIDJSON_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/rapidjson/include")
target_include_directories(Unofficial-Discord-Client SYSTEM PUBLIC "${RAPIDJSON_INCLUDE_PATH}")

# Add CPR
# Go away tests, go away!
set(BUILD_CPR_TESTS OFF)
add_subdirectory(external/cpr)
target_include_directories(Unofficial-Discord-Client SYSTEM PRIVATE "${CPR_INCLUDE_DIRS}")
target_link_libraries(Unofficial-Discord-Client "${CPR_LIBRARIES}")

# Add OpenSSL
find_package(OpenSSL REQUIRED)
target_include_directories(Unofficial-Discord-Client SYSTEM PRIVATE "${OPENSSL_INCLUDE_DIR}")
target_link_libraries(Unofficial-Discord-Client "${OPENSSL_LIBRARIES}")

# Use standalone asio
target_compile_definitions(Unofficial-Discord-Client PRIVATE USE_STANDALONE_ASIO ASIO_STANDALONE)

# Add SDL2
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(SDL2_LIB_PATH_PREFIX "x64")
else()
  set(SDL2_LIB_PATH_PREFIX "x86")
endif()

if (NOT WIN32)
  find_package(SDL2 REQUIRED)
elseif(MSVC)
  set(SDL2_LIBRARIES "${PROJECT_SOURCE_DIR}/external/sdl2-precompiled/msvc/libs/${SDL2_LIB_PATH_PREFIX}/SDL2.lib")
  set(SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/sdl2-precompiled/msvc/include") #TODO: change this to include/SDL2
elseif(MINGW)
  set(SDL2_LIBRARIES "${PROJECT_SOURCE_DIR}/external/sdl2-precompiled/mingw/libs/${SDL2_LIB_PATH_PREFIX}/libSDL2.a")
  set(SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/sdl2-precompiled/mingw/include") #TODO: change this to include/SDL2
endif()

target_include_directories(Unofficial-Discord-Client SYSTEM PRIVATE "${SDL2_INCLUDE_DIR}")
target_link_libraries(Unofficial-Discord-Client "${SDL2_LIBRARIES}")

# Add OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(Unofficial-Discord-Client "${OPENGL_gl_LIBRARY}")