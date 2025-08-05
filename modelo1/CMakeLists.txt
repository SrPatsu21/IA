cmake_minimum_required(VERSION 3.14)
project(ia_basica)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# GLOB todos os .cpp da pasta src/
file(GLOB SRC_FILES src/*.cpp)

add_executable(ia_basica ${SRC_FILES})

# Adiciona src/ e libs/eigen ao path de includes
target_include_directories(ia_basica PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/eigen
)
