cmake_minimum_required(VERSION 2.6)

set (PROJECT_NAME "snet-gateway" CXX)
project(${PROJECT_NAME})

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

if(MSVC)	
	#set(BOOST_ROOT C:/Boost) #specify coustom boost directory		
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
else()
	# Add custom cxx flags
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra")
endif()
# Automatically set the version number
set(SNET_GATEWAY_VERSION_MAJOR \"1\")
set(SNET_GATEWAY_VERSION_MINOR \"6pre\")
include_directories(
    "${PROJECT_BINARY_DIR}/src"
    "${PROJECT_SOURCE_DIR}/libs/framing/src"
    "${PROJECT_SOURCE_DIR}/libs/hdlcd-devel/src"
    "${PROJECT_SOURCE_DIR}/libs/snet-devel/src"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/src/Config.h.in"
    "${PROJECT_SOURCE_DIR}/src/Config.h"
)

add_subdirectory(src)
