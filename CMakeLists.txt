cmake_minimum_required(VERSION 3.11.0)

project(maze LANGUAGES C CXX VERSION 2.0)

# Seek Qt

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package( Threads )

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_subdirectory(src)
