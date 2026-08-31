cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_text_stream
    SRCS
        main.cpp
        text_stream_demo.h
        text_stream_demo.cpp
    QT_LIBS
        Qt5::Core
)
