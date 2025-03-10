cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)

project(C8Engine)

add_executable(C8Engine
	main.cpp

	include/C8Engine.h
	src/C8Engine.cpp
)

target_include_directories(C8Engine PRIVATE include)