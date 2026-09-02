cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_signalslot
    SRCS
        main.cpp
        main_window.h
        main_window.cpp
        main_window.ui
        worker_thread.h
        worker_thread.cpp
        custom_types.h
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
