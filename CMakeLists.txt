cmake_minimum_required(VERSION 3.20)

set(LinAlgebra_Sources
    Linalg.h
    LUDecomp.cpp
    LUDecomp.h
    Mtx.cpp
    Mtx.h
    Range.h
    Vect.cpp
    Vect.h
)

add_library(LinAlgebra ${LinAlgebra_Sources})

# BLAS for the products, exported with the LinAlgebra target to the programs using it
# (the headers include Linalg.h, which includes the BLAS header)
# on macOS, Linalg.h includes the Accelerate headers, so the BLAS library must be Accelerate too
if (APPLE)
    set(BLA_VENDOR Apple)
    target_compile_definitions(LinAlgebra PUBLIC LINALG_USE_ACCELERATE ACCELERATE_LAPACK_ILP64=1 ACCELERATE_NEW_LAPACK)
else()
    # cblas.h, e.g. of an OpenBLAS package given as BLAS_ROOT
    find_path(CBLAS_INCLUDE_DIR cblas.h HINTS ${BLAS_ROOT}/include PATH_SUFFIXES openblas)
    if (CBLAS_INCLUDE_DIR)
        target_include_directories(LinAlgebra PUBLIC ${CBLAS_INCLUDE_DIR})
    endif()
endif()

find_package(BLAS REQUIRED)
target_link_libraries(LinAlgebra PUBLIC ${BLAS_LIBRARIES} ${BLAS_LINKER_FLAGS})
