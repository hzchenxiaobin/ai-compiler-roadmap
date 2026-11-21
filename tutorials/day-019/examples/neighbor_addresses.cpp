#include <iostream>

// 观察栈上邻居的地址：连续声明的变量往往紧挨着
int main() {
    int a = 1;
    int b = 2;
    int c = 3;

    std::cout << "&a = " << &a << std::endl;
    std::cout << "&b = " << &b << std::endl;
    std::cout << "&c = " << &c << std::endl;

    std::cout << "&b - &a 相差 " << (&b - &a) << " 个 int（不是字节数！）" << std::endl;
    std::cout << "换成字节：(char*)&b - (char*)&a = "
              << (long)((char*)&b - (char*)&a) << " 字节" << std::endl;
    return 0;
}
