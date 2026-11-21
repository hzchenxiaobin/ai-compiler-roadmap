#include <iostream>
#include <string>

// 题二：FizzBuzz（LeetCode #412）
// 1~n：3 的倍数输出 Fizz，5 的倍数输出 Buzz，15 的倍数输出 FizzBuzz
std::string fizzbuzz(int n) {
    if (n % 15 == 0) {
        return "FizzBuzz";
    }
    if (n % 3 == 0) {
        return "Fizz";
    }
    if (n % 5 == 0) {
        return "Buzz";
    }
    return std::to_string(n);                 // to_string：数字转字符串
}

int main() {
    for (int n = 1; n <= 15; ++n) {
        std::cout << fizzbuzz(n);
        if (n < 15) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
