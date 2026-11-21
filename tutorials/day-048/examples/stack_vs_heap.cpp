#include <iostream>
#include <vector>

// Day 48 自测日素材一：白板讲清"栈和堆的区别"——程序化演示版
// 每个维度都配一段可运行的证据

// 维度三：生命期的证据（演示故意的悬空——pragma 压制教学警告）
int* dangling_demo() {
    int stack_local = 42;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-stack-address"
    return &stack_local;            // 返回栈变量地址——悬空！（教学演示）
#pragma clang diagnostic pop
}

int main() {
    std::cout << "== 维度一：申请与回收方式 ==" << std::endl;
    std::cout << "  栈：声明即分配、出块即回收（自动）" << std::endl;
    {
        int a = 1;                  // 声明即上栈
        int b[3] = {1, 2, 3};
        std::cout << "  栈变量 a @ " << &a << "，b @ " << b << std::endl;
    }                               // 出块即回收——零代码
    std::cout << "  堆：new 申请、delete 归还（手动）" << std::endl;
    int* p = new int(7);            // 手动申请
    std::cout << "  堆变量 *p @ " << p << "（在手里，得自己还）" << std::endl;
    delete p;                       // 手动归还

    std::cout << "\n== 维度二：大小与速度 ==" << std::endl;
    std::cout << "  栈：大小编译期定（默认 1~8MB）；回收 = 移指针（零成本）" << std::endl;
    std::cout << "  堆：大小运行期定（受内存上限）；分配走分配器（相对慢）" << std::endl;
    std::vector<int> big(1000000);  // 100 万个 int = 4MB——堆上（栈放不下！）
    std::cout << "  vector 100 万元素 @ " << big.data()
              << "（堆——栈上放不下的都在这）" << std::endl;

    std::cout << "\n== 维度三：生命期 ==" << std::endl;
    int* d = dangling_demo();
    std::cout << "  栈变量的地址返回后悬空：" << d << "（解引用是 UB）" << std::endl;
    int* heap_keep = new int(99);
    std::cout << "  堆变量跨块存活：" << heap_keep << "（等你 delete）" << std::endl;
    delete heap_keep;

    std::cout << "\n== 白板总结（五维对照） ==" << std::endl;
    std::cout << "  申请：栈自动｜堆手动（new）" << std::endl;
    std::cout << "  回收：栈出块即走（零成本）｜堆 delete（忘了=泄漏）" << std::endl;
    std::cout << "  大小：栈编译期+有限｜堆运行期+大" << std::endl;
    std::cout << "  生命期：栈块内｜堆跨块（到 delete）" << std::endl;
    std::cout << "  速度：栈快（移指针）｜堆慢（走分配器）" << std::endl;
    return 0;
}
