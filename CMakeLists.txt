cmake_minimum_required(VERSION 3.1)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

project(BSACalc) 
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)

SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/Install)
#IF(${CMAKE_INSTALL_CONFIG_NAME} MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
#    SET(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}.debug)
#ENDIF()

INSTALL( CODE "
    #IF( NOT \"\${CPACK_PACKAGE_FILE_NAME}\" STREQUAL \"\" )
        MESSAGE( STATUS \"Making a \${CMAKE_INSTALL_CONFIG_NAME} install area\" )
        #IF(\"\${CMAKE_INSTALL_CONFIG_NAME}\" MATCHES \"^([Dd][Ee][Bb][Uu][Gg])$\")
        #    SET(CMAKE_INSTALL_PREFIX \${CMAKE_INSTALL_PREFIX}.debug)
        #ENDIF()
        MESSAGE( STATUS \"Installing to \${CMAKE_INSTALL_PREFIX}\" )
     #ENDIF()
    ")

SET(CMAKE_AUTOMOC OFF)
SET(CMAKE_AUTORCC OFF)
SET(CMAKE_AUTOUIC OFF)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Modules ${CMAKE_MODULE_PATH})
#find_package(BSAQt5 REQUIRED)
#BSA_Install_Qt()
find_package(WinDeployQt REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
SET(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
WinDeployQt( BSACalc . )

INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION . )
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.pdb DESTINATION . CONFIGURATIONS Debug )
# INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/${PROJECT_NAME}.pdb DESTINATION . CONFIGURATIONS RelWithDebInfo )
# INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.pdb DESTINATION . CONFIGURATIONS Release )
# INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/MinSizeRel/${PROJECT_NAME}.pdb DESTINATION . CONFIGURATIONS MinSizeRel )

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

string(REPLACE "Source" ${CPACK_SYSTEM_NAME} PACKAGE_BASE_NAME ${CPACK_PACKAGE_FILE_NAME})

set(SRC_ZIP_FILE  ${CMAKE_BINARY_DIR}/${PACKAGE_BASE_NAME}.zip)
set(SRC_MSI_FILE  ${CMAKE_BINARY_DIR}/${PACKAGE_BASE_NAME}.exe)
set(DEST_ZIP_FILE ${CMAKE_SOURCE_DIR}/${PACKAGE_BASE_NAME}.zip)
set(DEST_MSI_FILE ${CMAKE_SOURCE_DIR}/${PACKAGE_BASE_NAME}.exe)

add_custom_command( 
    COMMENT "Creating Packages to Build area "
    OUTPUT ${SRC_ZIP_FILE} ${SRC_MSI_FILE}
    COMMAND ${CMAKE_COMMAND} --build . --target PACKAGE --config $<CONFIG>
    DEPENDS ${PROJECT_NAME}
    )

add_custom_command( 
    COMMENT "Copying Packages to Source area for GIT"
    OUTPUT ${DEST_ZIP_FILE} ${DEST_MSI_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy ${SRC_ZIP_FILE} ${DEST_ZIP_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy ${SRC_MSI_FILE} ${DEST_MSI_FILE}
    DEPENDS ${SRC_ZIP_FILE} ${SRC_MSI_FILE} 
    )

    
add_custom_target(INSTALL_PACKAGES
        DEPENDS ${DEST_ZIP_FILE} ${DEST_MSI_FILE} 
        )
set_target_properties(INSTALL_PACKAGES PROPERTIES FOLDER CMakePredefinedTargets)
