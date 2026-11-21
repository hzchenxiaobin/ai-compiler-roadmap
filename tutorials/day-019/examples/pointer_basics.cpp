#include <iostream>

// 指针第一课：取地址 &、解引用 *、通过指针改变量
int main() {
    int x = 42;
    int* p = &x;                 // p 是指针：存的是 x 的地址（& 是取地址）

    std::cout << "x 的值：" << x << std::endl;
    std::cout << "x 的地址 &x：" << &x << std::endl;
    std::cout << "指针 p 自己存的值：" << p << "（就是 x 的地址）" << std::endl;
    std::cout << "解引用 *p（顺着地址取东西）：" << *p << std::endl;

    *p = 99;                     // 顺着地址写东西：改的就是 x 本尊
    std::cout << "\n执行 *p = 99 之后：" << std::endl;
    std::cout << "x 的值：" << x << "（被指针改掉了！）" << std::endl;

    x = x + 1;                   // 直接改 x，*p 跟着变（同一个房间）
    std::cout << "执行 x = x + 1 之后：" << std::endl;
    std::cout << "*p 读到的：" << *p << std::endl;
    return 0;
}
