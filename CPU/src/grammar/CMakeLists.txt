add_library(grammar
    Rule.cpp
    LSystemGenerator.cpp
)

target_include_directories(grammar
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)