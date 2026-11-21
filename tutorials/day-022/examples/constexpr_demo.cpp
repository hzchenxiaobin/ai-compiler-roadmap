#include <iostream>

// const 与 constexpr：都能"只读"，区别在"什么时候定值"
int main() {
    int input = 0;
    std::cout << "输入一个数：";
    std::cin >> input;

    const int runtime_n = input;         // ✔ const：运行期定值也可以
    std::cout << "const runtime_n = " << runtime_n << std::endl;

    constexpr int compile_n = 5 * 5;     // ✔ constexpr：编译期就算好（25）
    std::cout << "constexpr compile_n = " << compile_n << std::endl;

    int table[compile_n];                // constexpr 可直接当数组长度
    table[0] = 100;
    std::cout << "table[0] = " << table[0] << "（长度 " << compile_n << "）" << std::endl;

    constexpr int bad = input;
    return 0;
}
