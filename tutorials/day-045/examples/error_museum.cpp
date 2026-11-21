#include <cstdlib>
#include <iostream>

// Day 45：内存错误案例集中剖析——四大错误各配一次 ASan 实验（计划指定）
// 用同一个程序逐个触发（每个 case 单独编译运行）

// 案例一：越界读写（heap-buffer-overflow）
void case1_overflow() {
    std::cout << "== 案例一：堆越界 ==" << std::endl;
    int* a = new int[3]{10, 20, 30};
    std::cout << "  合法：a[2] = " << a[2] << std::endl;
    std::cout << "  越界读：a[3] = " << a[3] << std::endl;   // 踩红区——ASan 现形
    delete[] a;
}

// 案例二：use-after-free
void case2_uaf() {
    std::cout << "== 案例二：use-after-free ==" << std::endl;
    int* p = new int(42);
    delete p;
    std::cout << "  悬空读：*p = " << *p << std::endl;       // 隔离区——ASan 现形
}

// 案例三：double-free
void case3_double_free() {
    std::cout << "== 案例三：double-free ==" << std::endl;
    int* p = new int(7);
    delete p;
    std::cout << "  第二次释放同一块……" << std::endl;
    delete p;                                               // 二进隔离区——ASan 现形
}

// 案例四：泄漏（macOS 的 ASan 不报——诚实记录；Linux/valgrind 可抓）
void case4_leak() {
    std::cout << "== 案例四：泄漏 ==" << std::endl;
    int* p = new int[100];                                  // 400 字节没人还
    p[0] = 1;
    std::cout << "  分配后没 delete——静默（macOS ASan 无报告；Linux 可抓）" << std::endl;
}

int main(int argc, char* argv[]) {
    int which = (argc > 1) ? std::atoi(argv[1]) : 0;
    switch (which) {
        case 1: case1_overflow(); break;
        case 2: case2_uaf(); break;
        case 3: case3_double_free(); break;
        case 4: case4_leak(); break;
        default:
            std::cout << "用法：./err <1~4>（每个案例单独跑）" << std::endl;
            std::cout << "  1=堆越界  2=use-after-free  3=double-free  4=泄漏" << std::endl;
    }
    return 0;
}
