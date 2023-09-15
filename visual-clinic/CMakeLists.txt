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
find_package(Qt6 REQUIRED COMPONENTS DataVisualization)

set(PROJECT_SOURCES
        src/main.cpp
        include/XMainWindow.h
        src/FileTab.cpp
        include/FileTab.h
        lib/rowfeature.cpp
        lib/math_common.h
        lib/leastsquare.cpp
        src/XGradientFrame.cpp
        include/XGradientFrame.h
        lib/covariance.cpp
        lib/pca.cpp
        lib/kmeans.cpp
        lib/dbscan.cpp
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
            include/index.h src/index.cpp index.ui
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
target_link_libraries(visual-clinic PRIVATE Qt6::DataVisualization)

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
