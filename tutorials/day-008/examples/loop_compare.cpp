#include <iostream>

// 同一个任务（打印 1~5）的三种循环写法对照
int main() {
    std::cout << "== for（已知次数首选） ==" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "== while（先判后做） ==" << std::endl;
    int j = 1;
    while (j <= 5) {
        std::cout << j << " ";
        ++j;
    }
    std::cout << std::endl;

    std::cout << "== do-while（先做后判，至少一次） ==" << std::endl;
    int k = 1;
    do {
        std::cout << k << " ";
        ++k;
    } while (k <= 5);
    std::cout << std::endl;
    return 0;
}
