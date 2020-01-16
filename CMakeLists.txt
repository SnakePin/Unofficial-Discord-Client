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

# Add our own CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

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

# Set required C++ version, this needs to be here, after the add_executable call
target_compile_features(Unofficial-Discord-Client PUBLIC cxx_std_17)

# Only add debug linker option to this project because we only need debug information for this project
if (WIN32 AND MSVC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  target_compile_options(Unofficial-Discord-Client PRIVATE "/W4")
  target_compile_options(Unofficial-Discord-Client PRIVATE "$<$<CONFIG:RELEASE>:/O2>")
  if(CMAKE_BUILD_TYPE EQUAL Debug OR CMAKE_BUILD_TYPE EQUAL RelWithDebInfo)
    set_target_properties(Unofficial-Discord-Client PROPERTIES LINK_FLAGS "/DEBUG:FULL")
  endif()
else()
  target_compile_options(Unofficial-Discord-Client PRIVATE "-Wall" "-Wextra")
  target_compile_options(Unofficial-Discord-Client PRIVATE "$<$<CONFIG:DEBUG>:-g>")
  target_compile_options(Unofficial-Discord-Client PRIVATE "$<$<CONFIG:RELEASE>:-O3>")
endif()

target_include_directories(Unofficial-Discord-Client PUBLIC "${PROJECT_SOURCE_DIR}/include")

# rapidjson include path
set(RAPIDJSON_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/rapidjson/include")
target_include_directories(Unofficial-Discord-Client SYSTEM PUBLIC ${RAPIDJSON_INCLUDE_PATH})

# Add CPR
# Go away tests, go away!
set(BUILD_CPR_TESTS OFF)
# Only enable HTTP support in libcurl
set(HTTP_ONLY ON)
add_subdirectory(external/cpr)
target_include_directories(Unofficial-Discord-Client SYSTEM PUBLIC ${CPR_INCLUDE_DIRS})
target_link_libraries(Unofficial-Discord-Client PUBLIC ${CPR_LIBRARIES})

# Set path variables for OpenSSL, OpenGL and SDL2 libraries
find_package(OpenSSL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
if(WIN32)
	foreach(DLL ${SDL2_DLLS})
		add_custom_command(TARGET Unofficial-Discord-Client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:Unofficial-Discord-Client>)
	endforeach()
endif()
set(LIBASIO_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/libasio/include")
set(SWS_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/sws")
set(TCLAP_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/tclap/include")
set(TINYFORMAT_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/tinyformat")

# Use standalone asio
target_compile_definitions(Unofficial-Discord-Client PRIVATE USE_STANDALONE_ASIO ASIO_STANDALONE)

# Add SWS and libasio includes as public
target_include_directories(Unofficial-Discord-Client SYSTEM PUBLIC ${LIBASIO_INCLUDE_PATH} ${SWS_INCLUDE_PATH})

# Add TCLAP, tinyformat, SDL2 and OpenSSL includes as private
target_include_directories(Unofficial-Discord-Client SYSTEM PRIVATE ${TINYFORMAT_INCLUDE_PATH} ${TCLAP_INCLUDE_PATH} ${SDL2_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

# Add SDL2, OpenGL and OpenSSL link libraries as private
target_link_libraries(Unofficial-Discord-Client PRIVATE ${SDL2_LIBS} ${OPENGL_LIBRARY} ${OPENSSL_LIBRARIES})
