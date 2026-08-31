cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_displays
    SRCS
        main.cpp
        main_window.h
        main_window.cpp
        main_window.ui
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
