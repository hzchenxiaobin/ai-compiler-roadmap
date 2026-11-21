#include <iostream>

// 自测题三（闭卷参考答案）：四种 const 组合各写一行并注释含义
int main() {
    int a = 10;
    int b = 20;

    int* p1 = &a;               // 都不锁：可改 *p1，可换 p1
    const int* p2 = &a;         // 锁物（const 在 * 左）：*p2 只读，p2 可换绑
    int* const p3 = &a;         // 锁针（const 在 * 右）：p3 终身指 a，*p3 可改
    const int* const p4 = &a;   // 双锁：*p4 只读且 p4 不换绑

    *p1 = 11;                   // ✔
    p2 = &b;                    // ✔（p2 现在看着 b）
    *p3 = 13;                   // ✔
    std::cout << "a = " << a << "（p1 改成 11，p3 又改成 13）" << std::endl;
    std::cout << "*p2 = " << *p2 << "（换绑后看 b）" << std::endl;
    std::cout << "*p4 = " << *p4 << "（双锁只读）" << std::endl;
    std::cout << "sizeof: p1=" << sizeof(p1) << "，p2=" << sizeof(p2)
              << "，p3=" << sizeof(p3) << "，p4=" << sizeof(p4) << "（都是 8——都是指针）" << std::endl;
    return 0;
}
