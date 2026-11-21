#include <iostream>
#include <iomanip>

// 第 14 天例子三：矩阵乘法——2×3 乘 3×2 得 2×2
// C[i][j] = A 的第 i 行 与 B 的第 j 列 对应相乘再求和
int main() {
    const int M = 2;                            // A 的行数
    const int K = 3;                            // A 的列数 = B 的行数（必须相等）
    const int N = 2;                            // B 的列数
    int a[M][K] = {
        {1, 2, 3},
        {4, 5, 6},
    };
    int b[K][N] = {
        { 7,  8},
        { 9, 10},
        {11, 12},
    };
    int c[M][N] = {{0}};                        // 结果先清零

    for (int i = 0; i < M; ++i) {               // 三重循环：结果共 M×N 个格子
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < K; ++k) {       // 每个格子 = K 对元素相乘求和
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    std::cout << "C = A × B：" << std::endl;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << std::setw(6) << c[i][j];
        }
        std::cout << std::endl;
    }
    return 0;
}
