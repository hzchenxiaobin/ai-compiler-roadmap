#include <iostream>
#include <iomanip>

// 数组指针：一个指针，指向"整行"（Day 14 矩阵的另一种走法）
int main() {
    int m[3][4] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12},
    };

    int (*row)[4] = m;        // row 指向第 0 行（整行 4 个 int 是一步的步长）

    std::cout << "用数组指针走矩阵：" << std::endl;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 4; ++c) {
            std::cout << std::setw(4) << row[r][c];   // row[r] 是第 r 行，再 [c] 取列
        }
        std::cout << std::endl;
    }

    std::cout << "row + 1 跳多远？" << std::endl;
    std::cout << "  row    = " << row << std::endl;
    std::cout << "  row+1  = " << (row + 1) << "（跳过整行 16 字节 = 4 个 int）" << std::endl;
    return 0;
}
