#include <iostream>

// 调试器练习场：单步跟踪一个循环的变量变化
int main() {
    int sum = 0;
    int product = 1;

    for (int i = 1; i <= 5; ++i) {
        sum += i;
        product *= i;
        std::cout << "i = " << i << "，sum = " << sum
                  << "，product = " << product << std::endl;
    }
    return 0;
}
