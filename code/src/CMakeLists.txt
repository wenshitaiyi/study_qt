# Automatically scan and add 3-level demo subdirectories with CMakeLists.txt
# Structure: code/src/<chapter>/<section>/<demo>/CMakeLists.txt

file(GLOB CHAPTER_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(CHAPTER ${CHAPTER_DIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${CHAPTER}")
        file(GLOB SECTION_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/${CHAPTER}" "${CMAKE_CURRENT_SOURCE_DIR}/${CHAPTER}/*")
        foreach(SECTION ${SECTION_DIRS})
            if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${CHAPTER}/${SECTION}")
                file(GLOB DEMO_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/${CHAPTER}/${SECTION}" "${CMAKE_CURRENT_SOURCE_DIR}/${CHAPTER}/${SECTION}/*")
                foreach(DEMO ${DEMO_DIRS})
                    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${CHAPTER}/${SECTION}/${DEMO}/CMakeLists.txt")
                        message(STATUS "Discovered Qt Demo: [${CHAPTER}/${SECTION}/${DEMO}]")
                        add_subdirectory("${CHAPTER}/${SECTION}/${DEMO}")
                    endif()
                endforeach()
            endif()
        endforeach()
    endif()
endforeach()
