#include <iostream>

// Day 37：打印不同变量的地址，观察内存区域差异
int global_var = 42;               // 全局变量：静态区
int global_zero = 0;               // 未显式初始化的全局（BSS，仍属静态区）
const char* const_str = "hello";   // 字符串字面量：常量区（只读）
static int static_var = 7;         // static 全局：仍是静态区

int main() {
    int stack_var = 10;            // 局部变量：栈
    int stack_arr[3] = {1, 2, 3};  // 局部数组：栈
    static int func_static = 99;   // 函数内 static：静态区（不进栈！）

    std::cout << "== 栈区（局部变量） ==" << std::endl;
    std::cout << "  &stack_var    = " << &stack_var << std::endl;
    std::cout << "  &stack_arr[0] = " << &stack_arr[0] << std::endl;
    std::cout << "  &stack_arr[2] = " << &stack_arr[2] << std::endl;

    std::cout << "\n== 静态区（全局/static） ==" << std::endl;
    std::cout << "  &global_var   = " << &global_var << std::endl;
    std::cout << "  &global_zero  = " << &global_zero << std::endl;
    std::cout << "  &static_var   = " << &static_var << std::endl;
    std::cout << "  &func_static  = " << &func_static << std::endl;

    std::cout << "\n== 常量区（字符串字面量） ==" << std::endl;
    std::cout << "  (void*)const_str = " << static_cast<const void*>(const_str) << std::endl;

    std::cout << "\n== 代码里的字面量地址对比 ==" << std::endl;
    const char* another = "world";
    const char* same = "hello";    // 编译器可能合并相同字面量
    std::cout << "  \"hello\" = " << static_cast<const void*>(const_str) << std::endl;
    std::cout << "  \"world\" = " << static_cast<const void*>(another) << std::endl;
    std::cout << "  \"hello\" 再取 = " << static_cast<const void*>(same)
              << (same == const_str ? "（与第一次相同——已合并）" : "（不同——未合并）") << std::endl;
    return 0;                       // 栈变量自动回收；静态区到进程结束
}
