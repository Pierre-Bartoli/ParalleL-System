add_library(IO
    Parser.cpp
)

target_include_directories(IO
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)