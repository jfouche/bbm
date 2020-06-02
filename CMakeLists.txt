cmake_minimum_required(VERSION 3.1.0)

project(bbm)

set(CMAKE_CXX_STANDARD 14)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)
    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)
endif()


add_subdirectory(src)
add_subdirectory(app)

# Testing only available if this is the main app
# Emergency override BBM_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR BBM_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(test)
endif()
