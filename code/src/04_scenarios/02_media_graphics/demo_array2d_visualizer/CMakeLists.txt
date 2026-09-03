cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_array2d_visualizer
    SRCS
        main.cpp
        main_window.h
        main_window.cpp
        control_panel.h
        control_panel.cpp
        visualizer_widget.h
        visualizer_widget.cpp
        minimap_widget.h
        minimap_widget.cpp
        grid_data_model.h
        grid_data_model.cpp
        noise_generator.h
        noise_generator.cpp
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Concurrent
        public_colourmap
)
