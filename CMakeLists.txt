cmake_minimum_required(VERSION 3.27)
project(S_Language C)

set(CMAKE_C_STANDARD 11)

include_directories("${PROJECT_SOURCE_DIR}/utils")
#include_directories("${PROJECT_SOURCE_DIR}/splitWords")
include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/scanner")

add_executable(S_Language main.c
        lib/openList.c
        scanner/scanner.c
        scanner/flood.c
        utils/utils.c
)
