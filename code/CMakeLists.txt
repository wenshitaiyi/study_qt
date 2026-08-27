# Third-party libraries if present
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rd/CMakeLists.txt")
    add_subdirectory(3rd)
endif()

# Source demos
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    add_subdirectory(src)
endif()
