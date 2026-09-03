cmake_minimum_required(VERSION 3.16)

add_library(public_colourmap STATIC
    colourmap.h
    colourmap.cpp
    colourmap_manager.h
    colourmap_manager.cpp
)

target_include_directories(public_colourmap PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(public_colourmap PUBLIC
    Qt5::Core
    Qt5::Gui
)
