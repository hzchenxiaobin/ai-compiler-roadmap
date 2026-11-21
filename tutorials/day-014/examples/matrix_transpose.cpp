#include <iostream>
#include <iomanip>

// 第 14 天例子二：矩阵转置——行列互换，3×4 变 4×3
int main() {
    const int ROWS = 3;
    const int COLS = 4;
    int m[ROWS][COLS] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12},
    };
    int t[COLS][ROWS];                          // 注意：转置后行列互换

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            t[c][r] = m[r][c];                  // 核心：行列对调
        }
    }

    std::cout << "原矩阵 3×4：" << std::endl;
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            std::cout << std::setw(4) << m[r][c];
        }
        std::cout << std::endl;
    }
    std::cout << "转置后 4×3：" << std::endl;
    for (int r = 0; r < COLS; ++r) {            // t 的行数是原列数
        for (int c = 0; c < ROWS; ++c) {        // t 的列数是原行数
            std::cout << std::setw(4) << t[r][c];
        }
        std::cout << std::endl;
    }
    return 0;
}
