cmake_minimum_required(VERSION 3.11)
project(frontend)

include_directories(include)
include_directories(src)

add_executable(frontend)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	target_compile_options(frontend PRIVATE /W4)
else ()
	target_compile_options(frontend PRIVATE -Wall)
endif ()

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
if (GLEW_FOUND)
	if (GLEW_LIBRARY_DEBUG)
		target_link_libraries(frontend debug ${GLEW_LIBRARY_DEBUG})
		target_link_libraries(frontend optimized ${GLEW_LIBRARY_RELEASE})
	else ()
		target_link_libraries(frontend ${GLEW_LIBRARIES})
	endif()
endif ()

# GL libraries
if (UNIX)
	find_library(GL_LIBRARY GL)
	target_link_libraries(frontend ${GL_LIBRARY})
else ()
	# Add raspberry specific GLES libraries here
endif ()

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR_freetype2})
if (FREETYPE_LIBRARY_DEBUG)
	target_link_libraries(frontend optimized ${FREETYPE_LIBRARY_RELEASE})
	target_link_libraries(frontend debug ${FREETYPE_LIBRARY_DEBUG})
else ()
	target_link_libraries(frontend ${FREETYPE_LIBRARY_RELEASE})
endif ()

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(frontend ${CURL_LIBRARY})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(frontend ${SDL2_LIBRARY})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(frontend ${SDL2_IMAGE_LIBRARY})

add_subdirectory("external")

target_link_libraries(frontend pugixml)
target_link_libraries(frontend nlohmann_json)

# C++ sources
file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp")
target_sources(frontend PRIVATE ${SOURCES})

# GLSL shaders
file(GLOB GLSL_FILES "shaders/*.glsl")
foreach(FILE ${GLSL_FILES})
	get_filename_component(FILE_NAME ${FILE} NAME)
	configure_file(${FILE} ${CMAKE_BINARY_DIR}/output/shader/${FILE_NAME} COPYONLY)
endforeach()
