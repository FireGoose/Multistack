cmake_minimum_required(VERSION 3.23)
project(stackqueue)
set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME stackqueue)
project(${PROJECT_NAME})

set(application StackQueueApplication)
set(library StackQueueLibrary)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(lib)
add_subdirectory(main)
add_subdirectory(gtest)
add_subdirectory(tests)
