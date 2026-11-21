#include <iostream>

// 撞名实验：两个命名空间各有一个 print_value
namespace lib_a {
void print_value(int v) { std::cout << "[A] " << v << std::endl; }
}

namespace lib_b {
void print_value(int v) { std::cout << "[B] " << v << std::endl; }
}

int main() {
    lib_a::print_value(1);        // 全限定：指名道姓，永无歧义
    lib_b::print_value(2);

    // 把两个都 using 进来再裸调？（预期编译失败：歧义）
    using lib_a::print_value;
    using lib_b::print_value;
    print_value(3);               // 歧义现场
    return 0;
}
