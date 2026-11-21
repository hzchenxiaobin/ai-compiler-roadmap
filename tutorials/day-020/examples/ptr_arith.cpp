#include <iostream>

// 指针运算：+1 跳多远，由指向的类型决定
int main() {
    int    ints[3]  = {1, 2, 3};
    double dbls[3]  = {1.5, 2.5, 3.5};

    std::cout << "int 数组：" << std::endl;
    std::cout << "  &ints[0] = " << &ints[0] << std::endl;
    std::cout << "  &ints[1] = " << &ints[1] << "（差 4 字节 = 1 个 int）" << std::endl;

    std::cout << "double 数组：" << std::endl;
    std::cout << "  &dbls[0] = " << &dbls[0] << std::endl;
    std::cout << "  &dbls[1] = " << &dbls[1] << "（差 8 字节 = 1 个 double）" << std::endl;

    // *(a + i) 与 a[i] 完全等价——下标就是指针运算的语法糖
    std::cout << "等价验证：" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "  ints[" << i << "] = " << ints[i]
                  << "，*(ints + " << i << ") = " << *(ints + i) << std::endl;
    }
    return 0;
}
