add_library(inih STATIC)

target_sources(inih PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/inih/ini.c"
    )

target_include_directories(inih PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/inih")
