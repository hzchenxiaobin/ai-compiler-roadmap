#include <iostream>

// i++ 与 ++i 的两张面孔
int main() {
    int i = 5;
    int a = i++;   // 后置：先把 i 的旧值交给 a，i 再自增
    std::cout << "a = " << a << ", i = " << i << std::endl;

    int b = ++i;   // 前置：i 先自增，再把新值交给 b
    std::cout << "b = " << b << ", i = " << i << std::endl;
    return 0;
}
