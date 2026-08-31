cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_datetime
    SRCS
        main.cpp
        datetime_demo.h
        datetime_demo.cpp
    QT_LIBS
        Qt5::Core
)
