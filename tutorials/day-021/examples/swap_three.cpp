#include <iostream>

// 主角：swap 三版本大战（值 / 指针 / 引用）
void swap_by_value(int a, int b) {      // 值传递：拿到的是复印件（Day 9）
    int t = a;
    a = b;
    b = t;
}

void swap_by_ptr(int* a, int* b) {      // 指针传递：拿到门牌，顺门牌去改
    int t = *a;
    *a = *b;
    *b = t;
}

void swap_by_ref(int& a, int& b) {      // 引用传递：拿到别名，改别名就是改本尊
    int t = a;
    a = b;
    b = t;
}

int main() {
    int x = 1, y = 2;

    swap_by_value(x, y);
    std::cout << "值版本之后：  x = " << x << "，y = " << y << "（纹丝不动）" << std::endl;

    swap_by_ptr(&x, &y);                // 注意：要传地址
    std::cout << "指针版本之后：x = " << x << "，y = " << y << "（换了！）" << std::endl;

    swap_by_ref(x, y);                  // 注意：直接传变量
    std::cout << "引用版本之后：x = " << x << "，y = " << y << "（又换回来！）" << std::endl;
    return 0;
}
