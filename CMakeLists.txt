cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)

project(YAC8I)

add_executable(YAC8I
	main.cpp

	include/C8Engine.h
	src/C8Engine.cpp
)

target_include_directories(YAC8I PRIVATE include olcPixelGameEngine)