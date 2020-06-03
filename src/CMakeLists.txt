set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(bbm
    datamodel.cpp
    datamodel_file.cpp
)

target_link_libraries(bbm PRIVATE Qt5::Widgets)
