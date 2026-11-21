#include <iostream>

// const 与指针的四种组合：谁被锁，读法见注释
int main() {
    int a = 10;
    int b = 20;

    int* p1 = &a;                    // 指针、指向物都不锁
    *p1 = 11;                        // ✔ 可以改指向物
    p1 = &b;                         // ✔ 可以换指向

    const int* p2 = &a;              // 【指向常量的指针】锁物不锁针
    // *p2 = 30;                     // ✘ 改指向物：编译错误（去 const 化给常量指针？）
    p2 = &b;                         // ✔ 可以换指向

    int* const p3 = &a;              // 【常量指针】锁针不锁物
    *p3 = 12;                        // ✔ 可以改指向物
    // p3 = &b;                      // ✘ 换指向：编译错误

    const int* const p4 = &a;        // 【双锁】针物全锁
    // *p4 = 40;                     // ✘ 编译错误
    // p4 = &b;                      // ✘ 编译错误

    std::cout << "a = " << a << "（被 p1 改成 11，又被 p3 改成 12）" << std::endl;
    std::cout << "p2 现在指向 b = " << *p2 << std::endl;
    std::cout << "p4 只读看着 a = " << *p4 << std::endl;
    return 0;
}
