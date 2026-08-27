cmake_minimum_required(VERSION 3.16)

project(study_qt LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Automated Build Tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# MSVC UTF-8 source and execution charset
if(MSVC)
    add_compile_options("/utf-8")
endif()

# Qt5 Dependency Prefix
if(DEFINED ENV{QTDIR} AND NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH $ENV{QTDIR})
endif()

# Default Qt Location on host if not set
if(NOT DEFINED CMAKE_PREFIX_PATH AND EXISTS "D:/app/qt/qt-5.15.2/5.15.2/msvc2019_64")
    set(CMAKE_PREFIX_PATH "D:/app/qt/qt-5.15.2/5.15.2/msvc2019_64")
endif()

# Find Core Qt5 Packages
find_package(Qt5 COMPONENTS Core Gui Widgets Network Sql Concurrent REQUIRED)
find_package(Qt5 COMPONENTS SerialPort Charts OPTIONAL_COMPONENTS)

# Unified Executable Output Directory: app/debug and app/release
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/app/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/app/release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/app/$<LOWER_CASE:$<CONFIG>>")

# Helper function to register a demo project
# Usage:
#   study_add_demo(demo_name
#       SRCS main.cpp ...
#       QT_LIBS Qt5::Core Qt5::Widgets ...
#   )
function(study_add_demo DEMO_NAME)
    cmake_parse_arguments(ARG "" "" "SRCS;QT_LIBS" ${ARGN})

    if(NOT ARG_SRCS)
        file(GLOB_RECURSE ARG_SRCS
            "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
            "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc"
        )
    endif()

    if(NOT ARG_QT_LIBS)
        set(ARG_QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets)
    endif()

    add_executable(${DEMO_NAME} ${ARG_SRCS})
    target_link_libraries(${DEMO_NAME} PRIVATE ${ARG_QT_LIBS})

    # Set project IDE folder hierarchy
    get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
    get_filename_component(SECTION_NAME "${PARENT_DIR}" NAME)
    get_filename_component(CHAPTER_DIR "${PARENT_DIR}/.." ABSOLUTE)
    get_filename_component(CHAPTER_NAME "${CHAPTER_DIR}" NAME)
    set_target_properties(${DEMO_NAME} PROPERTIES FOLDER "${CHAPTER_NAME}/${SECTION_NAME}")
endfunction()

# Enable IDE folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include source subtrees
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/code")
    add_subdirectory(code)
endif()
