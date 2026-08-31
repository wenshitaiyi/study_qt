cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_debug
    SRCS
        main.cpp
        logger.h
        logger.cpp
    QT_LIBS
        Qt5::Core
)
