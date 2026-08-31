cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_color
    SRCS
        main.cpp
        color_demo.h
        color_demo.cpp
    QT_LIBS
        Qt5::Core
        Qt5::Gui
)
