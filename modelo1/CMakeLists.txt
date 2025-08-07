cmake_minimum_required(VERSION 3.14)
project(ia_basica)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Linker flags (static linking for gcc/stdlibc++)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

# GLOB .cpp files
file(GLOB SRC_FILES src/*.cpp)

# Add executable
add_executable(ia_basica ${SRC_FILES})

# Include directories
target_include_directories(ia_basica PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/eigen
)

# Find and link OpenCV
find_package(OpenCV REQUIRED)

# It's better to use target-based commands than global ones
target_include_directories(ia_basica PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(ia_basica PRIVATE ${OpenCV_LIBS})