cmake_minimum_required(VERSION 3.16)

project(QtLottie LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(THORVG REQUIRED thorvg)


add_library(QtLottie SHARED
    include/qtlottie/LottieWidget.h
    src/LottieWidget.cpp
)

include_directories(${THORVG_INCLUDE_DIRS})

target_include_directories(QtLottie PRIVATE
    include/qtlottie/
)


target_link_libraries(QtLottie PRIVATE
 Qt${QT_VERSION_MAJOR}::Core
 Qt${QT_VERSION_MAJOR}::Gui
 Qt${QT_VERSION_MAJOR}::Widgets
)
link_directories(${THORVG_LIBRARY_LIBS})

target_compile_definitions(QtLottie PRIVATE TEST_LIBRARY)

