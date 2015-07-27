cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(whisk)

set(whisk_VERSION_MAJOR 2)
set(whisk_VERSION_MINOR 0)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(src)
