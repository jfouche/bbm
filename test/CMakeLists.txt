set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Tests need to be added as executables first
add_executable(test-bbm test-bbm.cpp)

find_package(Qt5 COMPONENTS Widgets Test REQUIRED)

target_include_directories(test-bbm
    PUBLIC ../src
)

target_link_libraries(test-bbm 
    PRIVATE bbm 
    PRIVATE Qt5::Widgets
    PRIVATE Qt5::Test
)

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME test-libbbm COMMAND test-bbm) # Command can be a target
