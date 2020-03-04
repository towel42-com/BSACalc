set(qtproject_SRCS
    main.cpp    
    NarcissisticNumbers/utils.cpp
    Calculator.cpp
)

set(qtproject_H
    Calculator.h
)

set(project_H
    NarcissisticNumbers/utils.h
)

set(qtproject_UIS
    Calculator.ui
)


set(qtproject_QRC
	application.qrc
)

file(GLOB qtproject_QRC_SOURCES "resources/*")
