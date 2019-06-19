# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (Unofficial-Discord-Client)

add_subdirectory(cpr)
include_directories(${CPR_INCLUDE_DIRS})
target_link_libraries(Unofficial-Discord-Client ${CPR_LIBRARIES})

# Add all cpp/c files
file(GLOB_RECURSE C_SOURCES "src/*.c")
file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")

# Normally this is not needed but for IDEs to see headers as part of the project they must be added too
file(GLOB_RECURSE C_HEADERS "include/*.h")
file(GLOB_RECURSE CPP_HEADERS "include/*.hpp")

# Add an executable with the above sources
add_executable(Unofficial-Discord-Client 
  ${CPP_SOURCES}
  ${C_SOURCES}
  ${C_HEADERS}
  ${CPP_HEADERS}
)
