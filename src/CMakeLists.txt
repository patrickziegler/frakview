find_package(SDL2 REQUIRED)

add_executable(frakview)

include_directories(${SDL2_INCLUDE_DIRS})

target_sources(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/frakview.c"
    )

target_link_libraries(frakview inih)
target_link_libraries(frakview ${SDL2_LIBRARIES})
