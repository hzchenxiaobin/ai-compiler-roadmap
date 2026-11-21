#include <iostream>

// 重写二：纯指针遍历（Day 20）——求和 + 最大
int main() {
    const int WEEKS = 8;
    int scores[WEEKS] = {12, 25, 9, 31, 18, 27, 22, 35};

    int sum = 0;
    int max_val = *scores;
    for (const int* p = scores; p != scores + WEEKS; ++p) {
        sum += *p;
        if (*p > max_val) {
            max_val = *p;
        }
    }
    std::cout << "总和 = " << sum << "，最大 = " << max_val << std::endl;
    return 0;
}
