add_library(grammar
    Rule.cpp
    LSystemGenerator.cpp
)

if(Kokkos_ENABLE_CUDA)
    set_source_files_properties(
        lSystemGenerator.cpp
        PROPERTIES LANGUAGE CUDA
    )
endif()

target_include_directories(grammar
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(grammar
    PUBLIC
        Kokkos::kokkos
)