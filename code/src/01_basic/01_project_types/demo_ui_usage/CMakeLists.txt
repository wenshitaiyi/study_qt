cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_ui_usage
    SRCS
        main.cpp
        main_window.h
        main_window.cpp
        main_window.ui
        panel_pointer_member.h
        panel_pointer_member.cpp
        panel_pointer_member.ui
        panel_direct_member.h
        panel_direct_member.cpp
        panel_direct_member.ui
        panel_multi_inherit.h
        panel_multi_inherit.cpp
        panel_multi_inherit.ui
        panel_pure_code.h
        panel_pure_code.cpp
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
