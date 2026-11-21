#include <iostream>
#include "my_utils.h"

// 同一个函数的三种用法（角色：分别注释，逐段启用实测）
int main() {
    // 用法一：全限定——最啰嗦也最安全
    std::cout << my_utils::clamp(150, 0, 100) << std::endl;
    std::cout << my_utils::to_percent(36, 144) << std::endl;

    // 用法二：using 声明——只放行一个名字
    using my_utils::clamp;
    std::cout << clamp(-7, 0, 100) << std::endl;   // 不用再写 my_utils::

    // 用法三：using 指令——整个命名空间敞开
    using namespace my_utils;
    std::cout << to_percent(72, 144) << std::endl;
    return 0;
}
