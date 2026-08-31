cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_smart_pointers
    SRCS
        main.cpp
        smart_pointers_demo.h
        smart_pointers_demo.cpp
    QT_LIBS
        Qt5::Core
)
