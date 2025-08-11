cmake_minimum_required(VERSION 3.10)
project(water_jug_tree LANGUAGES CXX)

# Versão do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opções de compilação padrão
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Mensagens úteis
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# Flags de warning (gcc/clang)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Habilitar sanitizers facilmente para Debug (opcional)
option(ENABLE_SANITIZERS "Enable Address/UB sanitizers in Debug" OFF)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Sanitizers enabled")
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

# Fonte (ajuste se o arquivo tiver outro nome)
set(SOURCES
    src/main.cpp
)

# Executável
add_executable(${PROJECT_NAME} ${SOURCES})

# Instalação simples (opcional)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin)

# Target 'run' para facilitar execução via CMake
add_custom_target(run
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME} --config $<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E echo ""
  COMMAND ${CMAKE_COMMAND} -E echo "---- Executando ${PROJECT_NAME} ----"
  COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Diretório de build sugerido ao usuário
message(STATUS "Para compilar: mkdir build && cd build && cmake .. && cmake --build .")
message(STATUS "Para rodar: cmake --build . --target run  (ou ./bin/${PROJECT_NAME})")