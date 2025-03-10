cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)

if(APPLE)
	add_subdirectory(3rdParty/raylib)
else()
	set(C8ENGINE_FILES include/OlcC8Engine.h include/OlcC8Engine.cpp)
endif()

project(YAC8I)

add_executable(YAC8I
	main.cpp

	include/C8Engine.h
	src/C8Engine.cpp

	${C8ENGINE_FILES}
)

target_include_directories(YAC8I PRIVATE include 3rdParty/olcPixelGameEngine)
target_link_libraries(YAC8I PRIVATE raylib)