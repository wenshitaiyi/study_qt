cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_xml
    SRCS
        main.cpp
        game_xml_models.h
        game_xml_models.cpp
        xml_stream_demo.h
        xml_stream_demo.cpp
    QT_LIBS
        Qt5::Core
        Qt5::Xml
)
