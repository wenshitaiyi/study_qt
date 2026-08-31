cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_list_widget
    SRCS
        main.cpp
        main_window.h
        main_window.cpp
        main_window.ui
        icon_helper.h
        icon_helper.cpp
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
