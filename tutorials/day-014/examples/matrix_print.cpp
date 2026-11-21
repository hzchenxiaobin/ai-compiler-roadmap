#include <iostream>
#include <iomanip>

// 第 14 天例子一：3 行 4 列矩阵的定义与双重循环打印
int main() {
    const int ROWS = 3;
    const int COLS = 4;
    int m[ROWS][COLS] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12},
    };

    for (int r = 0; r < ROWS; ++r) {           // 外层：一行一行走
        for (int c = 0; c < COLS; ++c) {       // 内层：一行内一列一列走
            std::cout << std::setw(4) << m[r][c];
        }
        std::cout << std::endl;                // 打完一行才换行（Day 7 的规矩）
    }
    return 0;
}
