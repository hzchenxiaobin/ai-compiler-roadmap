#include <iostream>

// 自测题二（闭卷参考答案）：仅用指针遍历，求和与最大值
int main() {
    const int WEEKS = 8;
    int scores[WEEKS] = {12, 25, 9, 31, 18, 27, 22, 35};

    int sum = 0;
    int max_val = *scores;                      // 首格当擂主（指针解引用取值）
    for (const int* p = scores; p != scores + WEEKS; ++p) {
        sum += *p;
        if (*p > max_val) {
            max_val = *p;
        }
    }

    std::cout << "总和 = " << sum << std::endl;
    std::cout << "最大 = " << max_val << std::endl;
    return 0;
}
