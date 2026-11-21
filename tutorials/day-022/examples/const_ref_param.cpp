#include <iostream>
#include <string>

// const 引用参数：不拷贝 + 承诺不改（从 Day 10 用到今天的写法今天讲透）
void print_banner(const std::string& text) {
    // text = "改不了";              // ✘ 编译错误：const 引用只读
    std::cout << "【" << text << "】（长度 " << text.size() << "）" << std::endl;
}

int main() {
    std::string greeting = "Hello, C++";
    print_banner(greeting);          // 不拷贝整个字符串，只递只读别名
    print_banner("字面量也行");       // const 引用还能绑字面量（普通引用不行）
    return 0;
}
