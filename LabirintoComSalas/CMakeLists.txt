cmake_minimum_required(VERSION 3.10)
project(maze_room LANGUAGES CXX)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# Compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Optional sanitizers for Debug
option(ENABLE_SANITIZERS "Enable Address/UB sanitizers in Debug" OFF)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Sanitizers enabled")
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

# Source files
set(SOURCES src/main.cpp)

# Set executable name depending on platform
if (WIN32)
    set(EXECUTABLE_OUTPUT_NAME "${PROJECT_NAME}")
else()
    set(EXECUTABLE_OUTPUT_NAME "${PROJECT_NAME}.out")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${EXECUTABLE_OUTPUT_NAME}")

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

# nlohmann/json
find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Copy room.json to build folder
configure_file(${CMAKE_SOURCE_DIR}/src/room.json
               ${CMAKE_CURRENT_BINARY_DIR}/room.json
               COPYONLY)

# Run target
add_custom_target(run
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME} --config $<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E echo ""
  COMMAND ${CMAKE_COMMAND} -E echo "---- Executando ${PROJECT_NAME} ----"
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_OUTPUT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)