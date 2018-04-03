cmake_minimum_required(VERSION 3.11)
project(frontend)

include_directories(include)
include_directories(src)

set(EXECUTABLE frontend)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_VERBOSE_MAKEFILE on)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
if (GLEW_FOUND)
if (GLEW_LIBRARY_DEBUG)
link_libraries(optimized ${GLEW_LIBRARY_RELEASE})
link_libraries(debug ${GLEW_LIBRARY_DEBUG})
else ()
link_libraries(${GLEW_LIBRARIES})
endif ()

# GL libraries
if (UNIX)
find_library(GL_LIBRARY GL)
link_libraries(${GL_LIBRARY})
endif ()
else ()
# Add raspberry specific GLES libraries here
endif ()

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR_freetype2})
if (FREETYPE_LIBRARY_DEBUG)
link_libraries(optimized ${FREETYPE_LIBRARY_RELEASE})
link_libraries(debug ${FREETYPE_LIBRARY_DEBUG})
else ()
link_libraries(${FREETYPE_LIBRARY_RELEASE})
endif ()

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
link_libraries(${CURL_LIBRARY})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
link_libraries(${SDL2_LIBRARY})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
link_libraries(${SDL2_IMAGE_LIBRARY})

# C++ sources
file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp")
add_executable(${EXECUTABLE} ${SOURCES})

# GLSL shaders
file(GLOB GLSL_FILES "src/shader/*.glsl")
foreach(FILE ${GLSL_FILES})
	get_filename_component(FILE_NAME ${FILE} NAME)
	configure_file(${FILE} ${CMAKE_BINARY_DIR}/output/shader/${FILE_NAME} COPYONLY)
endforeach()
