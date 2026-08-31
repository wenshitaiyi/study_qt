cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_dialog
    SRCS
        main.cpp
        main_dialog.h
        main_dialog.cpp
        main_dialog.ui
        custom_modal_dialog.h
        custom_modal_dialog.cpp
        custom_modal_dialog.ui
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
