cmake_minimum_required(VERSION 3.16)

project(CentreFormation VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql Charts PrintSupport)

qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    database/DatabaseManager.h
    database/DatabaseManager.cpp
    models/Salle.h
    models/Salle.cpp
    models/Cours.h
    models/Cours.cpp
    views/MainWindow.h
    views/MainWindow.cpp
    views/SalleWidget.h
    views/SalleWidget.cpp
    views/StatsSalleWidget.h
    views/StatsSalleWidget.cpp
    views/CoursWidget.h
    views/CoursWidget.cpp
    views/StatsCoursWidget.h
    views/StatsCoursWidget.cpp
)

qt_add_executable(CentreFormation
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_include_directories(CentreFormation PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(CentreFormation PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Charts
    Qt6::PrintSupport
)

set_target_properties(CentreFormation PROPERTIES
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(CentreFormation)
