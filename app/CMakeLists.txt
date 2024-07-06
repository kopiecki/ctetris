set(target "ctetris")
set(sources src/board.c src/tet.c src/main.c)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${target} ${sources})
target_link_libraries(${target} ${CURSES_LIBRARIES})

include(compiler/warnings-c)
compiler_warnings_install(${target})
