#include <iostream>

// Day 38：new / delete 基础——动态 int 与动态数组
int main() {
    std::cout << "== 动态 int ==" << std::endl;
    int* p = new int;              // 申请一个 int 的堆内存（值未初始化！）
    std::cout << "  未初始化：*p = " << *p << "（垃圾值）" << std::endl;
    *p = 42;                       // 手动赋值
    std::cout << "  赋值后：*p = " << *p << "，p = " << p << std::endl;
    delete p;                      // 释放——配对纪律第一条
    std::cout << "  已 delete（p 本身的值未变，但解引用是悬空——别用！）" << std::endl;

    std::cout << "\n== new 带初始化 ==" << std::endl;
    int* q = new int(7);           // 申请并初始化
    int* z = new int{};            // 值初始化（零）
    std::cout << "  new int(7)：*q = " << *q << std::endl;
    std::cout << "  new int{}：*z = " << *z << "（零初始化）" << std::endl;
    delete q;
    delete z;

    std::cout << "\n== 动态数组 ==" << std::endl;
    int n = 5;                     // 大小可以运行期才定——栈数组做不到！
    int* arr = new int[n]{};       // {} 全部零初始化
    for (int i = 0; i < n; ++i) { arr[i] = i * i; }
    for (int i = 0; i < n; ++i) {
        std::cout << "  arr[" << i << "] = " << arr[i]
                  << " @ " << (arr + i) << std::endl;
    }
    std::cout << "  相邻元素差 4 字节（int），地址连续" << std::endl;
    delete[] arr;                  // 数组用 delete[]——配对纪律第二条
    std::cout << "  已 delete[]" << std::endl;
    return 0;
}
