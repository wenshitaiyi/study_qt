cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_variant
    SRCS
        main.cpp
        game_types.h
        variant_demo.h
        variant_demo.cpp
    QT_LIBS
        Qt5::Core
)
