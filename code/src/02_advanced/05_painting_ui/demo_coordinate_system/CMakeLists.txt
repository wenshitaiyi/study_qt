cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_coordinate_system
    SRCS
        main.cpp
        main_window.h
        main_window.cpp
        main_window.ui
        transform_canvas.h
        transform_canvas.cpp
        transform_box.h
        transform_box.cpp
        custom_viewport.h
        custom_viewport.cpp
        primitive_shape.h
        primitive_shape.cpp
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
