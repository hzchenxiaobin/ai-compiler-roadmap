#include <iostream>

// 运算符陷阱巡礼：除法、取余、字符、优先级
int main() {
    std::cout << "== 整数除法 vs 浮点除法 ==" << std::endl;
    std::cout << "7 / 2           = " << 7 / 2 << std::endl;
    std::cout << "7.0 / 2         = " << 7.0 / 2 << std::endl;
    std::cout << "2 / 144         = " << 2 / 144 << std::endl;
    std::cout << "2 * 100.0 / 144 = " << 2 * 100.0 / 144 << std::endl;

    std::cout << std::endl << "== 取余的符号 ==" << std::endl;
    std::cout << "7 % 3  = " << 7 % 3 << std::endl;
    std::cout << "-7 % 3 = " << -7 % 3 << std::endl;
    std::cout << "7 % -3 = " << 7 % -3 << std::endl;

    std::cout << std::endl << "== 字符参与运算 ==" << std::endl;
    std::cout << "'A' + 1 = " << 'A' + 1 << std::endl;
    char next = 'A' + 1;
    std::cout << "char next = " << next << std::endl;

    std::cout << std::endl << "== 优先级与括号 ==" << std::endl;
    std::cout << "1 + 2 * 3   = " << 1 + 2 * 3 << std::endl;
    std::cout << "(1 + 2) * 3 = " << (1 + 2) * 3 << std::endl;
    return 0;
}
