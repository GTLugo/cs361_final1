# ===================================================
# SETUP
# ===================================================

cmake_minimum_required(VERSION 3.16)
project(cs361_project2)
set(CMAKE_CXX_STANDARD 20)
set(TARGET_NAME "mits")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/${CMAKE_SYSTEM_NAME})

# ===================================================
# COLLECT FILES
# ===================================================

set(SOURCE_FILES
    src/main.cpp
    src/core/simulation.cpp)

# ===================================================
# ADD EXECUTABLE TARGET
# ===================================================

add_executable(${TARGET_NAME}
    ${SOURCE_FILES})

set_target_properties(${TARGET_NAME}
    PROPERTIES
    OUTPUT_NAME ${TARGET_NAME})

if(MSVC)
  target_compile_options(${TARGET_NAME}
      PRIVATE
      /W4 /WX)
else()
  target_compile_options(${TARGET_NAME}
      PRIVATE
      -Wall -Wextra -Wpedantic -Werror)
endif()

target_include_directories(${TARGET_NAME}
    PRIVATE
    src)

target_precompile_headers(${TARGET_NAME}
    PUBLIC
    "src/simpch.hpp")