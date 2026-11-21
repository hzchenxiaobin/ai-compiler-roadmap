#include <iostream>

// 第 13 天主线例子：8 周刷题成绩的一站式统计
int main() {
    const int WEEKS = 8;                       // 数组长度用常量管起来
    int scores[WEEKS] = {12, 25, 9, 31, 18, 27, 22, 35};

    int max_val = scores[0];                  // 拿第 0 个当擂主，逐个挑战
    int min_val = scores[0];
    int sum = 0;
    for (int i = 0; i < WEEKS; ++i) {         // 标准遍历：i 从 0 到 WEEKS-1
        if (scores[i] > max_val) {
            max_val = scores[i];
        }
        if (scores[i] < min_val) {
            min_val = scores[i];
        }
        sum += scores[i];
    }

    std::cout << "8 周成绩：";
    for (int i = 0; i < WEEKS; ++i) {
        std::cout << scores[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "最大 = " << max_val << "（第 12 周）" << std::endl;
    std::cout << "最小 = " << min_val << "（第 3 周）" << std::endl;
    std::cout << "总和 = " << sum << std::endl;
    std::cout << "平均 = " << sum * 1.0 / WEEKS << std::endl;   // 乘 1.0 变浮点（Day 3）
    return 0;
}
