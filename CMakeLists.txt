## Konfiguration des Buildsystems CMake ##

# Minimale Version des Buildsystems
cmake_minimum_required(VERSION 3.14)
# Name des Projekts
project(Hangman)

## Optionen ##

# Setzte verwendeten C++-Standard auf C++17

# Prüfe ob Heapspeicher gefunden wird,
# der nicht freigegeben wurde
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# Füge selbsgeschriebene Includes hinzu
include_directories(include)
# Füge externe Includes hinzu
include_directories(external)

set(PYTHON_LIBRARY "external/python3.lib")
find_package(pybind11 REQUIRED)

## Baue Programme ##

# Baue das Programm 'demo1' aus den genannten Quelldateien
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(demo1
  examples/demo1.cpp
  ${SRC_FILES}
)

pybind11_add_module(battleship
  examples/pybind_battleship.cpp
  ${SRC_FILES}
)

## Installiere die Bibliothek
install(TARGETS battleship
  COMPONENT python
  LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/extra")

#----------------------------------------------#
#----------Download and use GoogleTest---------#
#----------------------------------------------#

# Use CMakes FetchContent
#include(FetchContent)
# Workaround for CMake >= 3.24
# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP
#if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
#  cmake_policy(SET CMP0135 NEW)
#endif()

# Download GoogleTest (we use version 1.12.1)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
# Workaround for Windows:
# Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Include CMake configuration of GoogleTest
#FetchContent_MakeAvailable(googletest)
#include(GoogleTest)

# Enable GoogleTest
#enable_testing()

#----------------------------------------------#
#-------------Erzeuge Testprogramme------------#
#----------------------------------------------#

# Declare test program battleship_board_tests
#add_executable(battleship_board_tests tests/battleship_board_tests.cpp ${SRC_FILES})
#target_link_libraries(battleship_board_tests GTest::gtest_main)
#gtest_discover_tests(battleship_board_tests)
