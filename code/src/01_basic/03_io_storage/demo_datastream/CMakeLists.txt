cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_datastream
    SRCS
        main.cpp
        project_model.h
        project_model.cpp
        data_stream_demo.h
        data_stream_demo.cpp
    QT_LIBS
        Qt5::Core
        Qt5::Gui
)
