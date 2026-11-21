#include <iostream>

// 自测题一（闭卷参考答案）：引用版 swap
void swap_ref(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

int main() {
    int x = 3, y = 8;
    std::cout << "交换前：x = " << x << "，y = " << y << std::endl;
    swap_ref(x, y);
    std::cout << "交换后：x = " << x << "，y = " << y << std::endl;
    return 0;
}
