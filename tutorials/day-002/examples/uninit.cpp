#include <iostream>

// 对照实验：未初始化的盒子 vs 初始化的盒子
int main() {
    int score;                // 只声明，未初始化：盒子是空的吗？
    std::cout << "未初始化的 score = " << score << std::endl;

    int safe_score = 0;       // 声明的同时初始化
    std::cout << "初始化后的 safe_score = " << safe_score << std::endl;
    return 0;
}
