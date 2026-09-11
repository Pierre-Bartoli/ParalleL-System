add_library(IO
    Parser.cpp
    SvgWriter.cpp
)

target_include_directories(IO
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)