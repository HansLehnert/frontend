cmake_minimum_required(VERSION 3.6)
project(frontend)

include_directories(include)
include_directories(src)

file(GLOB SOURCES "src/*.cpp")
file(GLOB SOURCES "src/*/*.cpp")

set(GLEW_DIR CACHE PATH "Location of GLEW")
set(SDL2_DIR CACHE PATH "Location of SDL2")
set(SOIL_DIR CACHE PATH "Location of SOIL")
set(CURL_DIR CACHE PATH "Location of CURL")

# glew
find_library(GLEW_LIB NAMES glew32 GLEW HINTS ${GLEW_DIR} PATH_SUFFIXES lib/Release/Win32)
if(GLEW_LIB)
	include_directories(${GLEW_DIR}/include)
	set(LIBRARIES ${LIBRARIES} ${GLEW_LIB})
else()
	message(FATAL_ERROR "GLEW not found")
endif()

# sdl2
find_library(SDL2_LIB NAMES SDL2 HINTS ${SDL2_DIR} PATH_SUFFIXES lib/x86)
if(SDL2_LIB)
	include_directories(${SDL2_DIR}/include)
	set(LIBRARIES ${LIBRARIES} ${SDL2_LIB})
else()
	message(FATAL_ERROR "SDL2 not found")
endif()

# soil
find_library(SOIL_LIB NAMES SOIL libsoil HINTS ${SOIL_DIR} PATH_SUFFIXES lib)
if(SOIL_LIB)
	include_directories(${SOIL_DIR}/include)
	set(LIBRARIES ${LIBRARIES} ${SOIL_LIB})
else()
	message(FATAL_ERROR "SOIL not found")
endif()

# curl
find_library(CURL_LIB NAMES curl HINTS ${CURL_DIR} PATH_SUFFIXES lib)
if(CURL_LIB)
	include_directories(${CURL_DIR}/include)
	set(LIBRARIES ${LIBRARIES} ${CURL_LIB})
else()
	message(FATAL_ERROR "SOIL not found")
endif()

add_executable(frontend ${SOURCES})
target_link_libraries(frontend ${LIBRARIES})
