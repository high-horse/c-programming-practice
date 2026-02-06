cmake_minimum_required(VERSION 3.16)
project(basic_gameloop_sdl C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_executable(a.out
        src/main.c
)

target_include_directories(a.out PRIVATE
        ${SDL2_INCLUDE_DIRS}
        src
)

target_link_libraries(a.out
        ${SDL2_LIBRARIES}
)
