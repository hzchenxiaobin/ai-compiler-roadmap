#include <iostream>

// 哨兵循环：一直读数求和，直到读到 0 为止
int main() {
    int sum = 0;
    int n = 0;

    std::cout << "逐个输入要累加的数（输入 0 结束）：" << std::endl;
    std::cin >> n;
    while (n != 0) {                            // 0 是哨兵：看到它就停
        sum += n;
        std::cin >> n;
    }
    std::cout << "总和 = " << sum << std::endl;
    return 0;
}
