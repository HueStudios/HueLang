cmake_minimum_required(VERSION 3.19.6)

project(HueLang)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(./)
add_subdirectory(lhuelang)
add_subdirectory(huelang)
add_subdirectory(huemodule.std)

