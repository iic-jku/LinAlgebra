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

set(LinAlgebra_Required_Libs
)

# on macOS, Linalg.h includes the Accelerate headers, so the BLAS library must be Accelerate too
if (APPLE)
    set(BLA_VENDOR Apple)
    add_compile_definitions(LINALG_USE_ACCELERATE ACCELERATE_LAPACK_ILP64=1 ACCELERATE_NEW_LAPACK)
endif()

find_package(BLAS REQUIRED)
include_directories(${BLAS_INCLUDE_DIRS})
set(LinAlgebra_Required_Libs ${LinAlgebra_Required_Libs} ${BLAS_LIBRARIES})

add_library(LinAlgebra ${LinAlgebra_Sources})

# libraries that needs to be linked to the present library
target_link_libraries(LinAlgebra ${LinAlgebra_Required_Libs})

