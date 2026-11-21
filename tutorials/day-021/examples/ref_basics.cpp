#include <iostream>

// 引用第一课：别名——同一间房间的第二个名字
int main() {
    int x = 42;
    int& r = x;                 // r 是 x 的引用（别名）：不是新盒子，是给 x 起的第二个名字

    std::cout << "x = " << x << "，r = " << r << std::endl;
    std::cout << "&x = " << &x << std::endl;
    std::cout << "&r = " << &r << "（同一个地址——同一个房间！）" << std::endl;

    r = 99;                     // 通过别名改，就是改本尊
    std::cout << "r = 99 之后：x = " << x << std::endl;

    x = x + 1;                  // 通过本名改，别名同步看到
    std::cout << "x = x + 1 之后：r = " << r << std::endl;

    // int& r2;                 // 取消注释会编译错误：引用必须初始化
    // int& r3 = nullptr;       // 取消注释也会报错：不存在空引用
    return 0;
}
