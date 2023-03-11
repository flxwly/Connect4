cmake_minimum_required(VERSION 3.24)
project(Connect4)

set(CMAKE_CXX_STANDARD 23)
set(SFML_DIR "C:\\GCC_Compiler\\SFML-2.5.1x32\\lib\\cmake\\SFML")
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)


configure_file(${CMAKE_SOURCE_DIR}/fonts/arial.ttf ${CMAKE_BINARY_DIR}/fonts/arial.ttf COPYONLY)
add_executable(Connect4 main.cpp GameWindow.cpp GameWindow.h GameLogic.cpp GameLogic.h AI.cpp AI.h)

target_link_libraries(Connect4 sfml-graphics)
