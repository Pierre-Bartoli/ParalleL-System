add_library(graphics
    Render.cpp
)

if(Kokkos_ENABLE_CUDA)
    set_source_files_properties(
        Render.cpp
        PROPERTIES LANGUAGE CUDA
    )
endif()

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(graphics
    PUBLIC
        Kokkos::kokkos
)