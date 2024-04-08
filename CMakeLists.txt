cmake_minimum_required(VERSION 3.5)

project(ChartModelTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Quick Widgets)

qt_standard_project_setup(REQUIRES 6.5)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        model.h model.cpp
        modelcontainer.h modelcontainer.cpp
        Chart.qml
)

qt_add_executable(ChartModelTestBin WIN32 MACOSX_BUNDLE
    ${PROJECT_SOURCES}
)

target_link_libraries(ChartModelTestBin
    PRIVATE
    Qt6::Charts
    Qt6::Quick
    Qt6::Widgets
)

qt_add_qml_module(ChartModelTestBin
    URI ChartModelTestQml
    QML_FILES
        "Chart.qml"
    RESOURCE_PREFIX /ChartModelTestQml
    NO_RESOURCE_TARGET_PATH
)

set_target_properties(ChartModelTestBin PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ChartModelTestBin
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
