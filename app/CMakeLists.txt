set(target "ctetris")
set(sources src/win.c src/board.c src/engine.c src/tet.c src/tetris.c
            src/main.c)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${target} ${sources})
target_link_libraries(${target} ${CURSES_LIBRARIES})
