cmake_minimum_required(VERSION 3.1)

project(BSACalc) 

SET(CMAKE_AUTOMOC OFF)
SET(CMAKE_AUTORCC OFF)
SET(CMAKE_AUTOUIC OFF)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

include( include.cmake )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

QT5_WRAP_UI(qtproject_UIS_H ${qtproject_UIS})
QT5_WRAP_CPP(qtproject_MOC_SRCS ${qtproject_H})
QT5_ADD_RESOURCES( qtproject_QRC_SRCS ${qtproject_QRC} )

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

source_group("Generated Files" FILES ${qtproject_UIS_H} ${qtproject_MOC_SRCS} ${qtproject_QRC_SRCS} )
source_group("Resource Files" FILES ${qtproject_QRC} ${qtproject_QRC_SOURCES} )
source_group("Designer Files" FILES ${qtproject_UIS} )
source_group("Header Files" FILES ${qtproject_H} )
source_group("CMake Files" FILES CMakeLists.txt include.cmake )

add_executable(BSACalc WIN32 ${project_SRCS} ${project_H}  
	${qtproject_SRCS} 
	${qtproject_QRC} 
	${qtproject_QRC_SRCS} 
	${qtproject_UIS_H} 
	${qtproject_MOC_SRCS} 
	${qtproject_H} 
	${qtproject_UIS}
	${qtproject_QRC_SOURCES}
	include.cmake
	CMakeLists.txt
)
target_link_libraries( BSACalc 
	Qt5::Widgets
    Qt5::Core
)

