find_package(SDL2 REQUIRED)

add_executable(frakview)

target_include_directories(frakview PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(frakview PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_sources(frakview PRIVATE
    "frakview.c"
    )

configure_file(
    "version.h.in"
    "version.h"
    )

target_link_libraries(frakview PRIVATE inih)
target_link_libraries(frakview PRIVATE ${SDL2_LIBRARIES})
