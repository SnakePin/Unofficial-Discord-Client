# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (Unofficial-Discord-Client)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(Unofficial-Discord-Client PRIVATE ${PROJECT_SOURCE_DIR}/include)

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
