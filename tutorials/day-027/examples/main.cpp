#include <iostream>
#include "calc.h"             // 拿到名片就能调用；机器在哪由链接器负责

int main() {
    std::cout << "3 + 4 = " << calc(3, '+', 4) << std::endl;
    std::cout << "10 / 4 = " << calc(10, '/', 4) << std::endl;
    std::cout << "运算符 'x' 合法吗？" << (is_valid_op('x') ? "是" : "否") << std::endl;
    return 0;
}
