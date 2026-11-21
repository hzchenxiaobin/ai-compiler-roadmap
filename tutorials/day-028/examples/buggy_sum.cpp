#include <iostream>

// 有 bug 的程序：想算 1+2+…+10 = 55，结果不对——用调试器抓虫
int main() {
    int total = 0;

    for (int i = 1; i < 10; ++i) {     // 疑点：条件是 < 还是 <=？
        total += i;
    }

    std::cout << "total = " << total << std::endl;
    return 0;
}
