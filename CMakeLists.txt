cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)

if(APPLE)
	set(C8ENGINE_FILES include/C8EnginePE.h src/C8EnginePE.cpp)
else()
	set(C8ENGINE_FILES include/C8EngineOLC.h src/C8EngineOCL.cpp)
endif()

project(YAC8I)

if(APPLE)
	find_package(PixelEngine REQUIRED)
endif()

add_executable(YAC8I
	main.cpp

	include/C8Engine.h
	src/C8Engine.cpp

	${C8ENGINE_FILES}
)

target_include_directories(YAC8I PRIVATE ${CMAKE_SOURCE_DIR}/include)
if(APPLE)
	target_include_directories(YAC8I PRIVATE ${PE_INCLUDE_DIRS})
	target_link_libraries(YAC8I PRIVATE ${PE_LIBRARIES})
else()
	target_include_directories(YAC8I PRIVATE 3rdParty/olcPixelGameEngine)
endif()