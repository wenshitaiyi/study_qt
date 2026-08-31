cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_string
    SRCS
        main.cpp
        string_demo.h
        string_demo.cpp
    QT_LIBS
        Qt5::Core
)
