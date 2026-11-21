#include <iostream>

// 重写一：swap 三版本（Day 9/19/21 的合订本）
void swap_by_value(int a, int b) { int t = a; a = b; b = t; }
void swap_by_ptr(int* a, int* b) { int t = *a; *a = *b; *b = t; }
void swap_by_ref(int& a, int& b) { int t = a; a = b; b = t; }

int main() {
    int x = 1, y = 2;
    swap_by_value(x, y);
    std::cout << "值版本后：　x=" << x << " y=" << y << std::endl;
    swap_by_ptr(&x, &y);
    std::cout << "指针版本后：x=" << x << " y=" << y << std::endl;
    swap_by_ref(x, y);
    std::cout << "引用版本后：x=" << x << " y=" << y << std::endl;
    return 0;
}
