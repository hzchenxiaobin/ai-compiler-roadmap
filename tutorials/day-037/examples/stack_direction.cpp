#include <iostream>

// 栈的生长方向与地址排列（本机观察）
void inner(int depth) {
    int local = depth;
    std::cout << "  depth " << depth << " 的局部变量地址：" << &local << std::endl;
    if (depth < 3) {
        inner(depth + 1);          // 递归：每层一个栈帧
    }
}

int main() {
    std::cout << "== 递归栈帧的地址排列 ==" << std::endl;
    inner(1);

    std::cout << "\n== 同函数内先声明的地址高还是低？ ==" << std::endl;
    int a = 1;
    int b = 2;
    std::cout << "  &a = " << &a << std::endl;
    std::cout << "  &b = " << &b << std::endl;
    std::cout << "  a 先声明，地址" << (&a > &b ? "高" : "低") << "于 b" << std::endl;
    std::cout << "  a-b 相差 " << (reinterpret_cast<char*>(&a) - reinterpret_cast<char*>(&b))
              << " 字节（int=4）" << std::endl;
    return 0;
}
