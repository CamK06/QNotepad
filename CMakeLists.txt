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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB SOURCES "src/*.cpp")

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(qnotepad ${SOURCES})
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Haiku")
target_link_libraries(qnotepad Qt5::Widgets spdlog fmt stdc++fs)
else()
target_link_libraries(qnotepad Qt5::Widgets spdlog fmt)
endif()

install(TARGETS qnotepad DESTINATION bin)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    install(FILES ${CMAKE_SOURCE_DIR}/qnotepad.desktop DESTINATION /usr/share/applications)
endif ()