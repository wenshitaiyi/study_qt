cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_metaobject
    SRCS
        main.cpp
        game_hero.h
        game_hero.cpp
    QT_LIBS
        Qt5::Core
)
