cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)

project(YAC8I)

find_package(PixelEngine)
if(NOT PE_FOUND)
	set(PixelEngine_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PixelEngine)
	include(${PixelEngine_DIR}/cmake/PixelEngineConfig.cmake)
	add_subdirectory(${PixelEngine_DIR})
endif()

add_definitions(-DROMS_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/chip8-roms/")

add_executable(YAC8I
	main.cpp

	include/C8Engine.h
	src/C8Engine.cpp

	include/C8EnginePE.h
	src/C8EnginePE.cpp
)

target_include_directories(YAC8I PRIVATE include ${PE_INCLUDE_DIRS})
target_link_libraries(YAC8I PRIVATE ${PE_LIBRARIES})
if(APPLE)
	target_link_libraries(YAC8I PRIVATE "-framework AudioToolBox")
endif()

install(DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/ DESTINATION .)

if(WIN32)
	set(YAC8I_RUNTIMES ${GLEW_RUNTIME} ${PE_RUNTIME})

	add_custom_command(
		  TARGET YAC8I
		  POST_BUILD
		  COMMAND ${CMAKE_COMMAND}
		  ARGS -E copy_if_different
		  ${YAC8I_RUNTIMES}
		  $<TARGET_FILE_DIR:YAC8I>
	  )
endif()