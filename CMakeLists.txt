cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)

project(YAC8I)

find_package(PixelEngine)
if(NOT PE_FOUND)
	set(PixelEngine_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PixelEngine)
	include(${PixelEngine_DIR}/cmake/PixelEngineConfig.cmake)
	add_subdirectory(${PixelEngine_DIR})
endif()

add_executable(YAC8I
	main.cpp

	include/C8Engine.h
	src/C8Engine.cpp

	include/C8EnginePE.h
	src/C8EnginePE.cpp
)

target_include_directories(YAC8I PRIVATE include ${PE_INCLUDE_DIRS})
target_link_libraries(YAC8I PRIVATE ${PE_LIBRARIES})