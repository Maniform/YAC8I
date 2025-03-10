cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)

if(APPLE)
	add_subdirectory(3rdParty/raylib)
endif()

project(YAC8I)

add_executable(YAC8I
	main.cpp

	include/C8Engine.h
	src/C8Engine.cpp
)

target_include_directories(YAC8I PRIVATE include 3rdParty/olcPixelGameEngine)
target_link_libraries(YAC8I PRIVATE raylib)