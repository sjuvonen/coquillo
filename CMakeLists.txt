cmake_minimum_required(VERSION 3.16)

project(Coquillo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS src/ui)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES Coquillo_fi_FI.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/app/currentmediapathdelegate.cpp
        src/app/currentmediapathdelegate.h
        src/app/mainwindow.cpp
        src/app/mainwindow.h
        src/app/mediastorage.cpp
        src/app/mediastorage.h
        src/app/mediastoragemodel.cpp
        src/app/mediastoragemodel.h
        src/app/mediastoragemodelcolumns.cpp
        src/app/mediastoragemodelcolumns.h
        src/app/mediastorageworker.cpp
        src/app/mediastorageworker.h
        src/app/selectionnotifier.cpp
        src/app/selectionnotifier.h
        src/app/settings.cpp
        src/app/settings.h
        src/app/filebrowser/filebrowser.cpp
        src/app/filebrowser/filebrowser.h
        src/app/filebrowser/filebrowsermodel.cpp
        src/app/filebrowser/filebrowsermodel.h
        src/app/processor/patternparse.cpp
        src/app/processor/patternparse.h
        src/app/processor/patternrename.cpp
        src/app/processor/patternrename.h
        src/app/tagseditor/mediaimages.cpp
        src/app/tagseditor/mediaimages.h
        src/app/tagseditor/mediametadata.cpp
        src/app/tagseditor/mediametadata.h
        src/app/tagseditor/mediatags.cpp
        src/app/tagseditor/mediatags.h
        src/core/files.cpp
        src/core/files.h
        src/core/finder.cpp
        src/core/finder.h
        src/core/media.cpp
        src/core/media.h
        src/core/type.h
        src/ui/filebrowser.ui
        src/ui/mainwindow.ui
        src/ui/patternparse.ui
        src/ui/patternrename.ui
        src/ui/mediaimages.ui
        src/ui/mediametadata.ui
        src/ui/mediatags.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Coquillo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Coquillo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Coquillo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Coquillo
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Coquillo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets tag)

target_include_directories(Coquillo PUBLIC . src/app src/ui)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Coquillo)
endif()
set_target_properties(Coquillo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "coquillo"
)

include(GNUInstallDirs)
install(TARGETS Coquillo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Coquillo)
endif()
