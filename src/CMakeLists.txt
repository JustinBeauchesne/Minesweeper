cmake_minimum_required(VERSION 3.20)
project(Minesweeper)
set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/Libraries/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(Minesweeper main.cpp GameState.h Tile.h Minesweeper.h Toolbox.h Button.h)
target_link_libraries(Minesweeper sfml-graphics sfml-audio)