cmake_minimum_required(VERSION 3.23)
project(L-system_GPU LANGUAGES C CXX CUDA)

#find_package(Kokkos REQUIRED)

include(FetchContent)

if(Kokkos_ENABLE_CUDA)
    set(
        Kokkos_ENABLE_COMPILE_AS_CMAKE_LANGUAGE ON CACHE BOOL "" FORCE
    )
endif()

FetchContent_Declare(
    Kokkos
    URL https://github.com/kokkos/kokkos/archive/refs/tags/5.2.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(Kokkos)

add_subdirectory(src/grammar)
add_subdirectory(src/IO)
add_subdirectory(src/graphics)

add_executable(L-system_GPU src/L-system_GPU.cpp)

if(Kokkos_ENABLE_CUDA)
    set_source_files_properties(
        src/L-system_GPU.cpp
        PROPERTIES LANGUAGE CUDA
    )
endif()

target_link_libraries(L-system_GPU
    PRIVATE
        grammar
        IO
        graphics
        Kokkos::kokkos
)