#include <iostream>

// 计划练习：仅用指针遍历数组求和（不用下标！）
int main() {
    const int WEEKS = 8;
    int scores[WEEKS] = {12, 25, 9, 31, 18, 27, 22, 35};

    int sum_index = 0;                                  // 下标版（对照组）
    for (int i = 0; i < WEEKS; ++i) {
        sum_index += scores[i];
    }

    int sum_pointer = 0;                                // 指针版（今天的主角）
    for (int* p = scores; p != scores + WEEKS; ++p) {   // 从首格走到尾格的下一格
        sum_pointer += *p;                              // 解引用取当前格的值
    }

    std::cout << "下标版总和 = " << sum_index << std::endl;
    std::cout << "指针版总和 = " << sum_pointer << std::endl;

    // 指针版逐格巡视：地址和值一起看
    std::cout << "指针巡视：" << std::endl;
    for (int* p = scores; p != scores + WEEKS; ++p) {
        std::cout << "  p = " << p << "，*p = " << *p << std::endl;
    }
    return 0;
}
