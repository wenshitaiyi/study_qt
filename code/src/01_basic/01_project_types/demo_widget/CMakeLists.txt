cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_widget
    SRCS
        main.cpp
        main_widget.h
        main_widget.cpp
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
