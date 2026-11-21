#include <iostream>

// 学习计划进度条：144 天走到哪了？
int main() {
    const int total = 144;
    int finished = 7;                 // 修改这个数，观察进度条变化
    const int bar_width = 36;         // 进度条总宽度：每格代表 4 天

    int full = finished * bar_width / total;        // 已完成：实心格数
    std::cout << "[";
    for (int i = 0; i < bar_width; ++i) {
        if (i < full) {
            std::cout << "#";
        } else {
            std::cout << ".";
        }
    }
    std::cout << "] " << finished << "/" << total << " 天" << std::endl;
    return 0;
}
