#include <iostream>
#include <iomanip>

// 九九乘法表：嵌套循环的经典
int main() {
    for (int row = 1; row <= 9; ++row) {          // 外层：第几行
        for (int col = 1; col <= row; ++col) {    // 内层：这一行乘到几
            std::cout << std::setw(4) << col << "×" << row << "="
                      << std::setw(2) << row * col;
        }
        std::cout << std::endl;                   // 一行打完才换行
    }
    return 0;
}
