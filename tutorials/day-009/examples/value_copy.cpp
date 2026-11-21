#include <iostream>

// 值传递：函数拿到的是实参的"复印件"
void double_it(int n) {
    n = n * 2;                                // 改的是复印件
    std::cout << "函数内 n = " << n << std::endl;
}

int main() {
    int x = 21;
    std::cout << "调用前 x = " << x << std::endl;
    double_it(x);                             // 把 x 复印一份交给函数
    std::cout << "调用后 x = " << x << std::endl;   // 原件纹丝不动
    return 0;
}
