cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_containers
    SRCS
        main.cpp
        container_demo.h
        container_demo.cpp
        pipeline_service.h
    QT_LIBS
        Qt5::Core
)
