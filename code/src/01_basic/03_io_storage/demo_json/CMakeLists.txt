cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_json
    SRCS
        main.cpp
        json_utils.h
        json_utils.cpp
        json_demo.h
        json_demo.cpp
    QT_LIBS
        Qt5::Core
)
