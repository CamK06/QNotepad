cmake_minimum_required(VERSION 3.5)
project(QNotepad)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_INSTALL_PREFIX "/usr/local")
else()
    set(CMAKE_INSTALL_PREFIX "/usr")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/fmt)

option(CLASSIC "Only build features present in the classic Windows notepad (WinXP era)" ON)

include_directories(src) # This shouldn't be needed, but we do it so ui_mainwindow.h can see editor.h

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB SOURCES "src/*.cpp")

find_package(QT NAMES Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
else()
  set(GIT_COMMIT "unknown")
endif()
add_definitions("-DGIT_COMMIT=\"${GIT_COMMIT}\"")

add_executable(qnotepad ${SOURCES})
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Haiku")
target_link_libraries(qnotepad Qt5::Widgets Qt5::PrintSupport spdlog fmt stdc++fs)
else()
target_link_libraries(qnotepad Qt5::Widgets Qt5::PrintSupport spdlog fmt)
endif()

install(TARGETS qnotepad DESTINATION bin)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    install(FILES ${CMAKE_SOURCE_DIR}/qnotepad.desktop DESTINATION /usr/share/applications)
endif ()