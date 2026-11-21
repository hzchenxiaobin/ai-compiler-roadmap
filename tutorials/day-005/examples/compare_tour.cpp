#include <iostream>

// 比较和逻辑运算符的结果是 bool：打印出来是 1 或 0
int main() {
    std::cout << (3 > 2) << std::endl;                    // 1：成立
    std::cout << (3 == 2) << std::endl;                   // 0：不成立
    std::cout << (3 != 2) << std::endl;                   // 1

    int today = 5;
    std::cout << (today >= 1 && today <= 6) << std::endl; // 1：两边都成立
    std::cout << (today == 6 || today == 7) << std::endl; // 0：两边都不成立
    std::cout << !(today == 6 || today == 7) << std::endl;// 1：取反
    return 0;
}
