cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_settings
    SRCS
        main.cpp
        app_settings.h
        app_settings.cpp
        settings_dialog.h
        settings_dialog.cpp
        settings_dialog.ui
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
