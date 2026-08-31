cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_geometry
    SRCS
        main.cpp
        geometry_demo.h
        geometry_demo.cpp
    QT_LIBS
        Qt5::Core
        Qt5::Gui
)
