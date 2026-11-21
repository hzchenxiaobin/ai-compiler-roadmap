#include <iostream>

// 正向与逆向遍历同一数组
int main() {
    const int WEEKS = 8;
    int scores[WEEKS] = {12, 25, 9, 31, 18, 27, 22, 35};

    std::cout << "正向（第 1 周 → 第 12 周）：";
    for (int i = 0; i < WEEKS; ++i) {
        std::cout << scores[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "逆向（第 12 周 → 第 1 周）：";
    for (int i = WEEKS - 1; i >= 0; --i) {    // 从最后一个下标 7 倒着数到 0
        std::cout << scores[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
