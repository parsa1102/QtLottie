cmake_minimum_required(VERSION 3.16)

project(QtLottie LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

add_executable(QtLottieExample
    main.cpp
)

include_directories(${THORVG_INCLUDE_DIRS})

link_directories(${THORVG_LIBRARY_DIRS})

target_link_libraries(QtLottieExample PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    QtLottie
    ${THORVG_LIBRARIES}
)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/Animation.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})