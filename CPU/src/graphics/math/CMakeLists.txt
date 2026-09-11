add_library(math
    Vec3.cpp
    Mat3.cpp
)

target_include_directories(math
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)