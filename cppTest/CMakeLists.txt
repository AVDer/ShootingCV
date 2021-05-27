cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O0 -Wall -Wextra")
set(CMAKE_VERBOSE_MAKEFILE ON)

project( ShootingCV )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( ShootingCV main.cpp target.cpp)
target_link_libraries( ShootingCV ${OpenCV_LIBS} )