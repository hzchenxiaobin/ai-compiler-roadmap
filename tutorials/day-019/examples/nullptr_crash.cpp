#include <iostream>

// nullptr：什么都不指的指针。解引用它 = 踩空（本程序预期崩溃）
int main() {
    int* p = nullptr;            // 空指针：明确表示"不指向任何东西"

    std::cout << "p = " << p << "（nullptr 打印为 0）" << std::endl;
    std::cout << "现在解引用空指针……" << std::endl;
    std::cout << *p << std::endl;            // 踩空！下面这行永远执行不到
    std::cout << "这行永远执行不到" << std::endl;
    return 0;
}
