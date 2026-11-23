#include <iostream>

// 星号金字塔：空格与星号的两层配合
int main() {
    const int height = 5;
    for (int row = 1; row <= height; ++row) {
        for (int space = 0; space < height - row; ++space) {
            std::cout << " ";                       // 左边的空格越来越少
        }
        for (int star = 0; star < 2 * row - 1; ++star) {
            std::cout << "*";                       // 星号：1、3、5、7、9
        }
        std::cout << std::endl;
    }
    return 0;
}
