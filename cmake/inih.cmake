add_library(inih STATIC)

target_sources(inih PRIVATE
    "${CMAKE_SOURCE_DIR}/extern/inih/ini.c"
    )

target_include_directories(inih PUBLIC "${CMAKE_SOURCE_DIR}/extern/inih")
