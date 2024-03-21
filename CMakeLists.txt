cmake_minimum_required(VERSION 3.27)
project(S_Language C)

set(CMAKE_C_STANDARD 11)

include_directories("${PROJECT_SOURCE_DIR}/utils")
#include_directories("${PROJECT_SOURCE_DIR}/splitWords")
include_directories("${PROJECT_SOURCE_DIR}/lib/open_list")
include_directories("${PROJECT_SOURCE_DIR}/lib/BinarySearchStringTree")
include_directories("${PROJECT_SOURCE_DIR}/scanner")

add_executable(S_Language main.c
        lib/open_list/open_list.c
        lib/BinarySearchStringTree/binary_search_string_tree.c
        scanner/scanner.c
        scanner/flood.c
        utils/utils.c
)
