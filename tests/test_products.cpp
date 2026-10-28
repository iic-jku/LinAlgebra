// Compares the BLAS-based products of LinAlgebra with the plain loops they replaced,
// on non-square, 1xN, Nx1, square and empty shapes.

#include <cmath>
#include <cstdio>
#include <initializer_list>

#include "Mtx.h"

static int failures = 0;

static CLin_Matrix makeMatrix(CLin_subscript m, CLin_subscript n, double seed)
{
    CLin_Matrix a(m, n);
    for (CLin_subscript i = 0; i < m; i++)
        for (CLin_subscript j = 0; j < n; j++)
            a[i][j] = seed + 1.0 * i - 0.5 * j + 0.25 * i * j;
    return a;
}

static CLin_Vector makeVector(CLin_subscript n, double seed)
{
    CLin_Vector x(n);
    for (CLin_subscript i = 0; i < n; i++)
        x[i] = seed - 0.75 * i;
    return x;
}

static void check(const char *what, CLin_subscript m, CLin_subscript k, CLin_subscript n,
                  CLin_subscript i, CLin_subscript j, double obtained, double expected)
{
    if (std::fabs(obtained - expected) > 1e-12 * (1.0 + std::fabs(expected))) {
        std::printf("%s (%lu x %lu) * (%lu x %lu): [%lu][%lu] = %g, expected %g\n",
                    what, m, k, k, n, i, j, obtained, expected);
        failures++;
    }
}

static void testMatrixMatrix(CLin_subscript m, CLin_subscript k, CLin_subscript n)
{
    CLin_Matrix a = makeMatrix(m, k, 1.0);
    CLin_Matrix b = makeMatrix(k, n, -2.0);
    CLin_Matrix c = matmult(a, b);
    CLin_Matrix d;
    matmult(d, a, b);
    if (c.num_rows() != m || c.num_cols() != n || d.num_rows() != m || d.num_cols() != n) {
        std::printf("matmult (%lu x %lu) * (%lu x %lu): wrong result dimensions\n", m, k, k, n);
        failures++;
        return;
    }
    for (CLin_subscript i = 0; i < m; i++)
        for (CLin_subscript j = 0; j < n; j++) {
            double sum = 0.0;
            for (CLin_subscript l = 0; l < k; l++)
                sum += a[i][l] * b[l][j];
            check("matmult(A, B)", m, k, n, i, j, c[i][j], sum);
            check("matmult(C, A, B)", m, k, n, i, j, d[i][j], sum);
        }
}

static void testMatrixVector(CLin_subscript m, CLin_subscript k)
{
    CLin_Matrix a = makeMatrix(m, k, 3.0);
    CLin_Vector x = makeVector(k, 0.5);
    CLin_Vector y = matmult(a, x);
    if (y.dim() != m) {
        std::printf("matmult (%lu x %lu) * (%lu x 1): wrong result dimension\n", m, k, k);
        failures++;
        return;
    }
    for (CLin_subscript i = 0; i < m; i++) {
        double sum = 0.0;
        for (CLin_subscript l = 0; l < k; l++)
            sum += a[i][l] * x[l];
        check("matmult(A, x)", m, k, 1, i, 0, y[i], sum);
    }
}

static void testDotProduct(CLin_subscript n)
{
    CLin_Vector x = makeVector(n, 1.5);
    CLin_Vector y = makeVector(n, -0.25);
    double sum = 0.0;
    for (CLin_subscript i = 0; i < n; i++)
        sum += x[i] * y[i];
    check("dot_prod(x, y)", 1, n, 1, 0, 0, dot_prod(x, y), sum);
}

int main()
{
    const CLin_subscript shapes[][3] = {
        {2, 3, 2}, {3, 2, 4}, {1, 5, 1}, {5, 1, 5}, {4, 4, 4}, {3, 7, 2}, {0, 3, 2}, {2, 0, 3}
    };
    for (const CLin_subscript *s : shapes) {
        testMatrixMatrix(s[0], s[1], s[2]);
        testMatrixVector(s[0], s[1]);
    }
    for (CLin_subscript n : {1, 7, 1000})
        testDotProduct(n);

    if (failures)
        std::printf("%d products differ from the loops\n", failures);
    return failures ? 1 : 0;
}
