cmake_minimum_required(VERSION 3.5)

project(visual-clinic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "D:\\Qt\\6.5.2\\mingw_64")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Charts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        mainwindow.ui
        include/XMainWindow.h
        src/FileTab.cpp
        include/FileTab.h
        lib/rowfeature.hpp
        lib/common.h
        lib/leastsquare.hpp
        src/XGradientFrame.cpp
        include/XGradientFrame.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(visual-clinic
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
            include/avgdialog.h src/avgdialog.cpp avgdialog.ui
            include/scatterdialog.h src/scatterdialog.cpp scatterdialog.ui
            include/relatedialog.h src/relatedialog.cpp relatedialog.ui
            include/pcadialog.h src/pcadialog.cpp pcadialog.ui
            include/meansdialog.h src/meansdialog.cpp meansdialog.ui
            include/attrdialog.h src/attrdialog.cpp attrdialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET visual-clinic APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(visual-clinic SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(visual-clinic
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(visual-clinic PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(visual-clinic PRIVATE Qt6::Charts)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.visual-clinic)
endif()
set_target_properties(visual-clinic PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS visual-clinic
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(visual-clinic)
endif()
