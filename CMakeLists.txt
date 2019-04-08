cmake_minimum_required(VERSION 3.9)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

project(frakview VERSION 1.0.0 LANGUAGES C)

include(inih)

add_subdirectory(src)
