cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

project(frakview C)

add_executable(${PROJECT_NAME})
include(src/CMakeLists.txt)

include(add_library_inih)
target_link_libraries(${PROJECT_NAME} inih)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lSDL2_image")

set(CMAKE_VERBOSE_MAKEFILE on)
