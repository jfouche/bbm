project(bbm-ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(app
    main.cpp
    mainwindow.cpp
    model_availablebbchildren.cpp
    model_bblist.cpp
    model_detail.cpp
    model_detail_usedby.cpp
    model_detail_uses.cpp
    model_projectbblist.cpp
    model_projectlist.cpp

    mainwindow.ui
)

target_include_directories(app
    PUBLIC ../src
)

target_link_libraries(app 
    PRIVATE bbm 
    PRIVATE Qt5::Widgets
)
