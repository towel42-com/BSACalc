cmake_minimum_required(VERSION 3.1)

project(BSACalc) 

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

include( include.cmake )

source_group("Generated Files" FILES ${qtproject_UIS_H} ${qtproject_MOC_SRCS} ${qtproject_QRC_SRCS} )
source_group("Resource Files" FILES ${qtproject_QRC} ${qtproject_QRC_SOURCES} )
source_group("Designer Files" FILES ${qtproject_UIS} )
source_group("Header Files" FILES ${qtproject_H} )

add_executable(BSACalc WIN32 ${project_SRCS} ${project_H}  
	${qtproject_SRCS} 
	${qtproject_QRC_SRCS} 
	${qtproject_UIS_H} 
	${qtproject_MOC_SRCS} 
	${qtproject_H} 
)
target_link_libraries( BSACalc 
	Qt5::Widgets
    Qt5::Core
)

