cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_file
    SRCS
        main.cpp
        file_demo.h
        file_demo.cpp
    QT_LIBS
        Qt5::Core
)
