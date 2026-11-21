#include <iostream>
#include <iomanip>

// 打印乘法表的某一行（第 row 行，乘到 row 为止）
void print_row(int row) {
    for (int col = 1; col <= row; ++col) {
        std::cout << std::setw(4) << col << "×" << row << "="
                  << std::setw(2) << row * col;
    }
    std::cout << std::endl;                   // 一行打完才换行
}

// 打印整张 n×n 乘法表
void print_mult_table(int n) {
    for (int row = 1; row <= n; ++row) {
        print_row(row);                       // 函数也可以调用函数
    }
}

// 打印学习进度条：已完成 finished / 共 total，条宽 bar_width 格
void print_progress_bar(int finished, int total, int bar_width) {
    int full = finished * bar_width / total;
    std::cout << "[";
    for (int i = 0; i < bar_width; ++i) {
        if (i < full) {
            std::cout << "#";
        } else {
            std::cout << ".";
        }
    }
    std::cout << "] " << finished << "/" << total << " 天" << std::endl;
}

int main() {
    print_mult_table(9);                      // main 只剩调度，一眼看懂
    print_progress_bar(9, 144, 36);
    print_progress_bar(72, 144, 36);
    return 0;
}
