#include <iostream>
#include <climits>

// 有符号整数溢出：平时悄悄绕回，UBSan 一开就现形
int main() {
    int big = INT_MAX;
    std::cout << "big = " << big << std::endl;
    int next_val = big + 1;   // 有符号溢出：未定义行为
    std::cout << "big + 1 = " << next_val << std::endl;
    return 0;
}
