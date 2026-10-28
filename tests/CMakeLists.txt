# compares the BLAS-based products with the plain loops they replaced
add_executable(test_products test_products.cpp)
target_include_directories(test_products PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(test_products LinAlgebra)
add_test(NAME linalgebra_products COMMAND test_products)
