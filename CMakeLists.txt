cmake_minimum_required(VERSION 3.1)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

project(BSACalc) 
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)

SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/Install)
#IF(${CMAKE_INSTALL_CONFIG_NAME} MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
#    SET(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}.debug)
#ENDIF()

INSTALL( CODE "
		MESSAGE( STATUS \"Making a \${CMAKE_INSTALL_CONFIG_NAME} install area\" )
		MESSAGE( STATUS \"Installing to \${CMAKE_INSTALL_PREFIX}\" )
    ")

SET(CMAKE_AUTOMOC OFF)
SET(CMAKE_AUTORCC OFF)
SET(CMAKE_AUTOUIC OFF)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Modules ${CMAKE_MODULE_PATH})

find_package(WinDeployQt REQUIRED)

include( include.cmake )
QT5_WRAP_UI(qtproject_UIS_H ${qtproject_UIS})
QT5_WRAP_CPP(qtproject_MOC_SRCS ${qtproject_H})
QT5_ADD_RESOURCES( qtproject_QRC_SRCS ${qtproject_QRC} )

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

source_group("Generated Files" FILES ${qtproject_UIS_H} ${qtproject_MOC_SRCS} ${qtproject_QRC_SRCS} )
source_group("Resource Files" FILES ${qtproject_QRC} ${qtproject_QRC_SOURCES} )
source_group("Designer Files" FILES ${qtproject_UIS} )
source_group("Header Files" FILES ${qtproject_H} ${project_H} )
SET( _CMAKE_FILES "CMakeLists.txt;include.cmake" )
source_group("CMake Files" FILES ${_CMAKE_FILES} )
FILE(GLOB _CMAKE_MODULE_FILES "${CMAKE_SOURCE_DIR}/Modules/*")
source_group("CMake Files\\Modues" FILES ${_CMAKE_MODULE_FILES} )

add_executable(BSACalc WIN32 ${project_SRCS} ${project_H}  
	${qtproject_SRCS} 
	${qtproject_QRC} 
	${qtproject_QRC_SRCS} 
	${qtproject_UIS_H} 
	${qtproject_MOC_SRCS} 
	${qtproject_H} 
	${qtproject_UIS}
	${qtproject_QRC_SOURCES}
	${_CMAKE_FILES}
	${_CMAKE_MODULE_FILES}
)
target_link_libraries( BSACalc 
	Qt5::Widgets
    Qt5::Core
)
SET(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)

WinDeployQt( BSACalc . )

INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION . )
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.pdb DESTINATION . CONFIGURATIONS Debug )

SET(CPACK_PACKAGE_VENDOR "BSA Troop 162")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_GENERATOR ZIP NSIS )
include(CPack)
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
