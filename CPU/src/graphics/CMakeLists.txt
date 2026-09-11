add_library(graphics
    Render.cpp
    Turtle.cpp
)

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)