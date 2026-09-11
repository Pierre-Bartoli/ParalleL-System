cmake_minimum_required(VERSION 3.23)
project(L-system_CPU LANGUAGES C CXX)

add_subdirectory(src/grammar)
add_subdirectory(src/IO)
add_subdirectory(src/graphics)
add_subdirectory(src/graphics/math)

add_executable(L-system_CPU src/L-system_CPU.cpp)

target_link_libraries(L-system_CPU
    PRIVATE
        grammar
        IO
        graphics
        math
)