#
# (c) Copyright 2004 - 2015 Blue Pearl Software Inc.
# All rights reserved.
#
# This source code belongs to Blue Pearl Software Inc.
# It is considered trade secret and confidential, and is not to be used
# by parties who have not received written authorization
# from Blue Pearl Software Inc.
#
# Only authorized users are allowed to use, copy and modify
# this software provided that the above copyright notice
# remains in all copies of this software.
#
#
# $Author: scott $ - $Revision: 52578 $ - $Date: 2019-04-26 15:01:37 -0700 (Fri, 26 Apr 2019) $
# $HeadURL: http://bpsvn/svn/trunk/Modules/FindBPSQt55.cmake $
#
#
#
INCLUDE(CheckCXXSourceCompiles)
SET(BSA_QT_FOUND FALSE)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets  )

get_target_property(QtCore_location Qt5::Core LOCATION)
IF( EXISTS ${QtCore_location} )
    set(AUTOGEN_TARGETS_FOLDER "Generated Files")

    MESSAGE( STATUS "QtCore: ${QtCore_location}" )

    IF(WIN32)
        STRING(REGEX REPLACE "/bin.*" "" QTDIR_TMP ${QtCore_location})
    ELSE()
        STRING(REGEX REPLACE "/lib/libQ.*" "" QTDIR_TMP ${QtCore_location})
    ENDIF()
    
    SET( QT_VERSION_MAJOR ${Qt5Core_VERSION_MAJOR} )
    SET( QT_VERSION_MINOR ${Qt5Core_VERSION_MINOR} )
    SET( QT_VERSION_PATCH ${Qt5Core_VERSION_PATCH} )
                
    SET(QTDIR ${QTDIR_TMP} CACHE STRING "The QT Directory")
    message(STATUS "Looking in ${QTDIR} for QT shared library files.")
    SET(BSA_QT_FOUND TRUE)

    if ( WIN32 )
        IF( MSVC12 )
            STRING(REGEX REPLACE ".*/msvc" "msvc" MSVC_DIR ${QtCore_location})
            STRING(REGEX REPLACE "/bin.*" "" MSVC_DIR ${MSVC_DIR})
            IF(CMAKE_SIZEOF_VOID_P EQUAL 4) #32 bit
                IF( NOT ${MSVC_DIR} STREQUAL "msvc2013")
                    MESSAGE( STATUS "MSVC_DIR: ${MSVC_DIR}" )
                    MESSAGE( STATUS "Attempting to generate a Visual Studio Project for a 32 bit VS 2013" )
                    MESSAGE( STATUS "However, Qt is NOT pointing to a 32 bit VS 2013 area" )
                    MESSAGE( STATUS "If this is correct, please clear your CMake Cache and re-run cmake" )
                    MESSAGE( STATUS "Otherwise, please make sure your path is pointing to the correct qmake" )
                    MESSAGE( STATUS "Also, make sure to run cmake with -G \"Visual Studio 12 2013\" to set the correct 32 bit generator" )
                    MESSAGE( STATUS "If you want to generate a project for 64 bit, run cmake -G \"Visual Studio 12 2013 Win64\"" )
                    MESSAGE( FATAL_ERROR "See above for issues and solution" )
                ENDIF()
            ELSE()
                IF( NOT ${MSVC_DIR} STREQUAL "msvc2013_64")
                    MESSAGE( STATUS "MSVC_DIR: ${MSVC_DIR}" )
                    MESSAGE( STATUS "Attempting to generate a Visual Studio Project for a 64 bit VS 2013" )
                    MESSAGE( STATUS "However, Qt is NOT pointing to a 64 bit VS 2013 area" )
                    MESSAGE( STATUS "If this is correct, please clear your CMake Cache and re-run cmake" )
                    MESSAGE( STATUS "Otherwise, please make sure your path is pointing to the correct qmake" )
                    MESSAGE( STATUS "Also, make sure to run cmake with -G \"Visual Studio 12 2013 Win64\" to set the correct 64 bit generator" )
                    MESSAGE( STATUS "If you want to generate a project for 32 bit, run cmake -G \"Visual Studio 12 2013\"" )
                    MESSAGE( FATAL_ERROR "See above for issues and solution" )
                ENDIF()
            ENDIF()
        ENDIF()
    ENDIF()

    function(BSA_QT5_ADD_RESOURCES outfilesBase outputSuffixes )
        foreach(outputSuffix ${outputSuffixes})
            set(outfiles ${outfilesBase}${outputSuffix})
            set(options)
            set(oneValueArgs)
            set(multiValueArgs OPTIONS)

            cmake_parse_arguments(_RCC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

            set(rcc_files ${_RCC_UNPARSED_ARGUMENTS})
            set(rcc_options ${_RCC_OPTIONS})

            if("${rcc_options}" MATCHES "-binary")
                message(WARNING "Use qt5_add_binary_resources for binary option")
            endif()

            foreach(it ${rcc_files})
                get_filename_component(outfilename ${it} NAME_WE)
                get_filename_component(infile ${it} ABSOLUTE)
                set(outfile ${CMAKE_CURRENT_BINARY_DIR}/bsa_qrc_${outfilename}${outputSuffix}.cpp)

                _QT5_PARSE_QRC_FILE(${infile} _out_depends _rc_depends)
           
                #MESSAGE( "${it} => ${outfile}" )

                add_custom_command(OUTPUT ${outfile}
                                   COMMAND ${Qt5Core_RCC_EXECUTABLE}
                                   ARGS ${rcc_options} --name ${outfilename} --output ${outfile} ${infile}
                                   MAIN_DEPENDENCY ${infile}
                                   DEPENDS ${_rc_depends} "${out_depends}" VERBATIM)
                list(APPEND ${outfiles} ${outfile})
            endforeach()
            set(${outfiles} ${${outfiles}} PARENT_SCOPE)
        endforeach()
    endfunction()
                                                                                                        


    FUNCTION(AddQtIncludes ModuleName )
        if ( DEFINED Qt5${ModuleName}_INCLUDE_DIRS )
            foreach( dir ${Qt5${ModuleName}_INCLUDE_DIRS} )
                include_directories( ${dir} )
            endforeach()
        else()
            message( WARNING "Qt Module: ${ModuleName} is not defined (possibly not loaded)" )
        endif()
    ENDFUNCTION()

    MACRO(CheckFile isSet fileName )
            IF( NOT EXISTS ${fileName} )
                message( FATAL_ERROR "Could not find Qt library ${fileName}" )
            ENDIF()
    ENDMACRO()


    MACRO(BSA_Install_Qt_libWin isSet libName )
        IF(${ARGN}) #negative logic
            IF( ${isSet} )
                SET( INSTALL_IT FALSE )
            ELSE()
                SET( INSTALL_IT TRUE )
            ENDIF()
        ELSE()
            SET( INSTALL_IT ${isSet} )
        ENDIF()
        IF( ${INSTALL_IT} )
            IF(WIN32)
                    message( STATUS "Installing Qt library ${libName}" )
                    install(PROGRAMS ${QTDIR}/bin/${libName}d.dll DESTINATION . CONFIGURATIONS Debug )
                    install(PROGRAMS ${QTDIR}/bin/${libName}d.pdb DESTINATION . CONFIGURATIONS Debug )
                    install(PROGRAMS ${QTDIR}/bin/${libName}.dll DESTINATION . CONFIGURATIONS RelWithDebInfo Release MinSizeRel)

                    CheckFile( ${isSet} ${QTDIR}/bin/${libName}d.dll )
                    CheckFile( ${isSet} ${QTDIR}/bin/${libName}d.pdb )
                    CheckFile( ${isSet} ${QTDIR}/bin/${libName}.dll )
            ENDIF()
        ELSE()
            #message( STATUS "Skipping Installation of Qt library ${libName}" )
        ENDIF()
    ENDMACRO()

    MACRO(BSA_Install_Qt_lib  isSet libName )
        BSA_Install_Qt_libWin( ${isSet} ${libName} ${ARGN} )
    ENDMACRO()

    MACRO(BSA_Install_Qt_pluginWin  isSet dir libName )
        IF(${ARGN}) #negative logic
            IF( ${isSet} )
                SET( INSTALL_IT FALSE )
            ELSE()
                SET( INSTALL_IT TRUE )
            ENDIF()
        ELSE()
            SET( INSTALL_IT ${isSet} )
        ENDIF()
        IF( ${INSTALL_IT} )
            IF(WIN32)
                    message( STATUS "Installing Qt plugin ${dir}/${libName}" )
                    install(PROGRAMS ${QTDIR}/plugins/${dir}/${libName}d.dll DESTINATION ${dir} CONFIGURATIONS Debug )
                    install(PROGRAMS ${QTDIR}/plugins/${dir}/${libName}d.pdb DESTINATION ${dir} CONFIGURATIONS Debug )
                    install(PROGRAMS ${QTDIR}/plugins/${dir}/${libName}.dll DESTINATION ${dir} CONFIGURATIONS RelWithDebInfo Release MinSizeRel )

                    CheckFile( ${isSet} ${QTDIR}/plugins/${dir}/${libName}d.dll )
                    CheckFile( ${isSet} ${QTDIR}/plugins/${dir}/${libName}d.pdb )
                    CheckFile( ${isSet} ${QTDIR}/plugins/${dir}/${libName}.dll )
        ENDIF()
            ELSE()
                #message( STATUS "Skipping Installation of Qt plugin ${dir}/${libName}" )
        ENDIF()
    ENDMACRO()

    
    MACRO(BSA_Install_Qt_plugin  isSet dir libName )
        BSA_Install_Qt_pluginWin( ${isSet} ${dir} ${libName} ${ARGN} )
    ENDMACRO()

    MACRO(BSA_Install_Qt_libX  isSet libName )
        IF(${ARGN}) #negative logic
            IF( ${isSet} )
                SET( INSTALL_IT FALSE )
            ELSE()
                SET( INSTALL_IT TRUE )
            ENDIF()
        ELSE()
            SET( INSTALL_IT ${isSet} )
        ENDIF()
        IF( ${INSTALL_IT} )
            IF(WIN32)
                    message( STATUS "Installing Qt library ${libName}" )
                    install(PROGRAMS ${QTDIR}/bin/${libName}.dll DESTINATION . )

                    CheckFile( ${isSet} ${QTDIR}/bin/${libName}.dll )
            ENDIF()
        ENDIF()
    ENDMACRO()

    MACRO(BSA_Install_Qt)
		BSA_Install_Qt_lib( QT_DONT_USE_QTCORE Qt5Core TRUE )
		BSA_Install_Qt_lib( QT_DONT_USE_QTGUI Qt5Gui TRUE )
		BSA_Install_Qt_lib( QT_DONT_USE_QTGUI Qt5Widgets TRUE )
		# BSA_Install_Qt_lib( QT_DONT_USE_QTGUI Qt5Sensors TRUE )
		BSA_Install_Qt_lib( QT_DONT_USE_QTGUI Qt5PrintSupport TRUE )
		# BSA_Install_Qt_libX( QT_DONT_USE_QTGUI icudt54 TRUE )
		# BSA_Install_Qt_libX( QT_DONT_USE_QTGUI icuin54 TRUE )
		# BSA_Install_Qt_libX( QT_DONT_USE_QTGUI icuuc54 TRUE )
		# BSA_Install_Qt_libWin( QT_DONT_USE_QTGUI libEGL TRUE )
		# BSA_Install_Qt_libWin( QT_DONT_USE_QTGUI libGLESv2 TRUE )
		BSA_Install_Qt_lib( QT_USE_XML_PATTERNS Qt5XmlPatterns )
		BSA_Install_Qt_lib( QT_USE_XML Qt5Xml )
		#BSA_Install_Qt_lib( QT_USE_QTDESIGNER Qt5Designer )
		#BSA_Install_Qt_lib( QT_USE_QTDESIGNER Qt5DesignerComponents )
		#BSA_Install_Qt_lib( QT_USE_QTMOTIF NotSupported )
		BSA_Install_Qt_lib( QT_USE_QTMULTIMEDIA Qt5Multimedia )
		BSA_Install_Qt_lib( QT_USE_QTNETWORK Qt5Network )
		# BSA_Install_Qt_lib( QT_USE_QTNSPLUGIN NotSupported )
		BSA_Install_Qt_lib( QT_USE_QTOPENGL Qt5OpenGL )
		BSA_Install_Qt_lib( QT_USE_QTSQL Qt5Sql )
		BSA_Install_Qt_lib( QT_USE_QTSVG Qt5Svg )
		BSA_Install_Qt_lib( QT_USE_QTTEST Qt5Test )
		#BSA_Install_Qt_lib( QT_USE_QTUITOOLS NotSupported )
		#BSA_Install_Qt_lib( QT_USE_QTDBUS NotSupported )
		BSA_Install_Qt_lib( QT_USE_QTSCRIPT Qt5Script )
		#BSA_Install_Qt_lib( QT_USE_QTASSISTANTCLIENT NotSupported )
		BSA_Install_Qt_lib( QT_USE_QTHELP Qt5Help )
		BSA_Install_Qt_lib( QT_USE_QTWEBKIT Qt5WebChannel )
		BSA_Install_Qt_lib( QT_USE_QTWEBKIT Qt5WebKit )
		BSA_Install_Qt_lib( QT_USE_QTWEBKIT Qt5WebKitWidgets )
		BSA_Install_Qt_lib( QT_USE_QTWEBKIT Qt5Multimedia )
		BSA_Install_Qt_lib( QT_USE_QTWEBKIT Qt5MultimediaWidgets )
		BSA_Install_Qt_lib( QT_USE_QTWEBKIT Qt5OpenGL )
		BSA_Install_Qt_lib( QT_USE_QTWEBKIT Qt5Positioning )
		BSA_Install_Qt_lib( QT_USE_QTWEBKIT Qt5Qml )
		BSA_Install_Qt_lib( QT_USE_QTWEBKIT Qt5Quick )
		#BSA_Install_Qt_lib( QT_USE_PHONON phonon )
		BSA_Install_Qt_lib( QT_USE_QTSCRIPTTOOLS Qt5ScriptTools )
		BSA_Install_Qt_lib( QT_USE_QTDECLARATIVE Qt5Declarative )
		BSA_Install_Qt_pluginWin( QT_DONT_USE_QTGUI platforms qwindows TRUE)
		BSA_Install_Qt_pluginWin( QT_DONT_USE_QTGUI platforms qminimal TRUE)
		BSA_Install_Qt_pluginWin( QT_DONT_USE_QTGUI printsupport windowsprintersupport TRUE )
		BSA_Install_Qt_pluginWin( QT_USE_QTSQL sqldrivers qsqlite)
    ENDMACRO()
ELSE()
    message( FATAL_ERROR "Could not find directory QtCore_location=${QtCore_location}" )
ENDIF()
