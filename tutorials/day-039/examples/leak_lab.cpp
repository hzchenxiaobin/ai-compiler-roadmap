#include <iostream>

// Day 39：写一个泄漏程序并用工具定位（计划指定练习）
// 三种泄漏形态 + 工具实测见 README
void leak_fixed_size() {
    int* p = new int[100];            // 形态一：固定大小，每次调用漏 400 字节
    p[0] = 1;                          // 用了一下
}                                      // p 出块消失——内存没人管了

void leak_growing() {
    static int n = 1;
    n *= 2;
    int* p = new int[n];               // 形态二：越漏越大（指数级）
    p[0] = n;
}

int* make_dangling() {
    int local = 42;
    int* p = new int(local);
    delete p;                          // 正确释放了……
    return p;                          // 但返回的指针指向已释放内存（悬空）
}

int main() {
    std::cout << "== 泄漏的累积（普通编译看不见） ==" << std::endl;
    for (int i = 0; i < 1000; ++i) {
        leak_fixed_size();             // 1000 次 × 400 字节 = 400KB 没了
    }
    for (int i = 0; i < 10; ++i) {
        leak_growing();                // 2+4+8+...+1024 ≈ 2046 个 int
    }
    std::cout << "  跑完 1010 次分配，全部没还——程序正常退出 exit=0" << std::endl;
    std::cout << "  （没有任何报错——这就是'静默'的可怕）" << std::endl;

    std::cout << "\n== 悬空指针 ==" << std::endl;
    int* d = make_dangling();
    std::cout << "  d = " << d << "（值还在，但指向已释放内存）" << std::endl;
    std::cout << "  *d = " << *d << "（悬空读取——UB：普通版可能打出旧值 4 的假象）" << std::endl;
    return 0;
}
