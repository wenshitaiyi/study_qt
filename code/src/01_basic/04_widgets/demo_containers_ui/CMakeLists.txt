cmake_minimum_required(VERSION 3.16)

study_add_demo(demo_containers_ui
    SRCS
        main.cpp
        main_window.h
        main_window.cpp
        main_window.ui
        page_groupbox.h
        page_groupbox.cpp
        page_groupbox.ui
        page_tabwidget.h
        page_tabwidget.cpp
        page_tabwidget.ui
        page_toolbox.h
        page_toolbox.cpp
        page_toolbox.ui
        page_scrollarea.h
        page_scrollarea.cpp
        page_scrollarea.ui
        page_splitter.h
        page_splitter.cpp
        page_splitter.ui
    QT_LIBS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
