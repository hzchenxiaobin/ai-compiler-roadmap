#include <iostream>

// 配对纪律的反面教材：三种典型错误（只演示不模仿）
int main() {
    std::cout << "== 错误一：忘记 delete（泄漏） ==" << std::endl;
    {
        int* leak = new int[100];     // 申请 400 字节
        leak[0] = 1;                  // 用了一下
        std::cout << "  出块时没 delete——这 400 字节到进程结束都拿不回来" << std::endl;
        std::cout << "  （编译器不报错！静默泄漏——Day 39 的工具课主角）" << std::endl;
    }                                  // leak 指针没了，内存还占着

    std::cout << "\n== 错误二：delete 后继续用（悬空） ==" << std::endl;
    int* d = new int(10);
    delete d;                          // 内存已还
    // std::cout << *d;                // ✘ 悬空解引用（UB）——Day 39 主题
    std::cout << "  delete 后 p 的值还在（ dangling 指针），但 *p 是 UB" << std::endl;
    d = nullptr;                       // 好习惯：delete 后置空
    std::cout << "  置空后 if (d) 检查能防误用" << std::endl;

    std::cout << "\n== 错误三：new[] 配 delete（错配） ==" << std::endl;
    int* arr = new int[3]{1, 2, 3};
    // delete arr;                     // ✘ 应该用 delete[]——UB（ASan 能抓）
    delete[] arr;                      // ✔ 正确配对
    std::cout << "  new[] 必须配 delete[]（元素的析构只被 [] 版逐个调用）" << std::endl;
    return 0;
}
